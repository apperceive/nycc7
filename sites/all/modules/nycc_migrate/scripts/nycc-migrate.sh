#!/bin/bash

####################################################
#
# NYCC migration from Drupal 6 to 7
#
# copy database and file from production to source
# perform migration from source to target
#
# assumes base vars in .conf file in same folder
# as this command script
#
####################################################

curuser=`whoami`

shdir="$(dirname "$0")"

migconf=$shdir/`echo $(basename $0)| sed -e 's/\.sh$//'`.conf 

if [ -n "$1" ] ; then
  if ! [[ "$1" == -* ]] ; then 
    migconf="$1"
  fi
fi

if [ -n "$2" ] ; then
  if ! [[ "$2" == -* ]] ; then 
    migconf="$2"
  fi
fi

if [ -e $migconf ] 
then
  echo "Using config: $migconf"
else
  # TODO: if no folder specified, try looking for conf in users home or in #shdir
  echo "Unable to locate config file: $migconf"
fi

source "$migconf"

productiondb="nycc"
productionfilesdir="/var/www/html/nycc/sites/default"
productionssh="/home/$produser/.ssh/id_rsa"
productionuser="$produser@nycc.org"
productionuser="$produser@nycc.org"
productiontmpdir="/tmp"
productionssh="/home/$produser/.ssh/id_rsa"

if [ -z "$referencealias" ]
then
  # alias for reference site containing target init database (configuration)
  referencealias="@d7Test"
fi
# NOTE: assumes no spaces in any of these values or they are removed
rds=`drush $referencealias status`
referencerootdir=`echo "$rds" | grep "Drupal root" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
referencesite=`echo "$rds"  | grep "Site path" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
# note: all commands must append /files to $referencedir for safety
referencedir="$referencerootdir/$referencesite"

sds=`drush $sourcealias status`
sourcedb=`echo "$sds" | grep "Database name" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
sourcerootdir=`echo "$sds" | grep "Drupal root" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
sourcesite=`echo "$sds" | grep "Site path" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
# note: all commands must append /files to $sourcedir for safety
sourcedir="$sourcerootdir/$sourcesite"

tds=`drush $targetalias status`
targetdb=`echo "$tds" | grep "Database name" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
targetrootdir=`echo "$tds" | grep "Drupal root" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
targetsite=`echo "$tds" | grep "Site path" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
# not currently used, but also seems to be loaded with multi-lined string?!?!
# targetprivatedir=`echo "$tds" | grep "Private file directory path" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
# targetpublicdir=`echo "$tds" | grep "File directory path" | awk -F: '{ print $2 }'`
# note: all commands must append /files to $targetdir for safety
targetdir="$targetrootdir/$targetsite"

tmpdir=`echo "$tds" | grep "Temporary file directory path" | awk -F: '{ print $2 }' | sed -e 's/ //g'`
scriptsdir="$targetrootdir/sites/all/modules/nycc_migrate/scripts/migrate"
logfile="$tmpdir/migrate-$curuser.log"
timestamp="`date +%Y-%m-%d_%H-%M`"

# command aliases
mysql='mysql -uroot -pXt2792b8cf'
mysqldump='mysqldump -uroot -pXt2792b8cf'
mysqlexec="drush $targetalias scr $scriptsdir/sqlexec.php --sourcedb=$sourcedb"
fieldcopy="drush $targetalias scr $scriptsdir/field_copy.php --sourcedb=$sourcedb --trace"

# TODO: sanity check all vars. use an 'are you sure?' prompt

# USAGE - HELP

function nycc_migrate_usage () {
    echo "
Usage: $(basename $0) [-options] [conf]
    -m -1           migration init
    -b -2           backups (source and target)
    -p -3           production export and sync
    -d -4           import production to source
    -c -5           clear out target database and files before migration
    -a -6           copy source to target
    -w -7           cleanup source
    -x -8           cleanup target
    -y -9           cleanup migration (and source)
    -s              display migration status
    -t -0           run test script
    -v              verbose (nothing extra yet)
    -i              source report (trivial)
    -j              target report (trivial)
    -h              this usage help text
    -S              sync reference site to target prior to migration
    conf            alternate configuration file (default is nycc_migrate.conf)
Migrate NYCC Drupal 6 to 7
Examples:
    $(basename $0) [conf] -123456789"
    exit ${1:-0}
}

# MIGRATION FUNCTIONS

# INITIALIZE MIGRATION
# TODO: sanity check all folders and settings, issue warnings if need be
# NOTE: this is only for migration objects, not source or target - assume no backup yet
function nycc_migrate_init_migration() {
  echo "Migration init..."
  mkdir -p $tmpdir/nycc_migrate
  echo "NYCC Migration - $timestamp"
  # TODO: display config/options
  # echo "$@"
  echo "nycc_migrate_init_migration complete."
}

# EXPORT PRODUCTION DATABASE, RYSYNC DATABASE AND FILES TO TARGET
function nycc_migrate_backup_source_and_target() {

  echo "Removing old temporary backups ($tmpdir/*.sql.bz2)..."
  sudo rm -fR $tmpdir/nycc_migrate/*
  
  echo "Making (bzip2) backup of source database..."
  $mysqldump $sourcedb | bzip2 -c > $tmpdir/nycc_migrate/$sourcedb-$timestamp.sql.bz2

  echo "Making (bzip2) backup of target database..."
  $mysqldump $targetdb | bzip2 -c > $tmpdir/nycc_migrate/$targetdb-$timestamp.sql.bz2
  
  # TODO: backup sourcedirs and targetdir's
  echo "nycc_migrate_backup_source_and_target complete."
}

function nycc_migrate_export_production() {
  echo "Exporting production database..."
  
  # TODO: use a command script on production to limit number of calls or combine here
  
  ssh $productionuser "rm -f $productiontmpdir/production.sql"  
  ssh $productionuser "$mysqldump $productiondb > $productiontmpdir/production.sql"
  
  echo "Rsyncing production database export..."
  sudo rm -f $tmpdir/production.sql
  sudo rsync -z -e "ssh -i $productionssh" $productionuser:$productiontmpdir/production.sql $tmpdir
  sudo chmod 775 $tmpdir/production.sql

  echo "Removing export from production temp folder"
  ssh $productionuser "rm -f $productiontmpdir/production.sql"
  
  echo "nycc_migrate_export_production complete."
}

function nycc_migrate_sync_production_to_source() {

  echo "Clearing out source files folder..."
  find $sourcedir/files -maxdepth 1 -type f -exec sudo rm -f {} \;
  find $sourcedir/files -maxdepth 1 -type d -not -name "files" -exec sudo rm -R -f {} \;
  
  echo "Rsyncing production files to source (update)..."
  sudo rsync -azu -e "ssh -i $productionssh" --exclude="backup_migrate/scheduled"  --exclude="backup_migrate/manual" --exclude="styles" --exclude="js" --exclude="css" --exclude="imagecache" --exclude="ctools" --exclude="files"  --exclude="print_pdf" --exclude="imagefield_thumbs" $productionuser:$productionfilesdir/files/ $sourcedir/files

  # NOTE: these modules give us errors in drush so turn them off
  # NOTE: some are a problem on the site too, so leave off?
  # TODO: move this to a source_init function
  
  echo "Deactivating specific source modules (NOT TODO?)..."
  
  #drush $sourcealias dis -y  ms_product_kit , ms_products, ms_core ms,_authorizenet, ms_bonus_pack, ms_authorizenet, ms_bonus_pack, ms_check, ms_membership, ms_paypal_wpp, ms_paypal_wps, ms_reports_2, ms_stripe 
  
  #drush $sourcealias pmu -y ms_product_kit, ms_products, ms_core, ms_authorizenet, ms_bonus_pack, ms_authorizenet, ms_bonus_pack, ms_check, ms_membership, ms_paypal_wpp, ms_paypal_wps, ms_reports_2, ms_stripe
  
  #$mysql $sourcedb -e"UPDATE system SET status = 0 WHERE system.name IN ('nycc_email', 'rules', 'watchdog_rules', 'logging_alerts', 'nycc_ipn', 'ms_core');"

  echo "nycc_migrate_sync_production_to_source complete."
}

# IMPORT PRODUCTION TO SOURCE DATABASE
function nycc_migrate_import_production_to_source() {
  # CHECK: production.sql file exists in $tmpdir
  if [ -e $tmpdir/production.sql ] 
  then
    echo "Clear source database..."
    $mysql -e"DROP DATABASE IF EXISTS $sourcedb; CREATE DATABASE $sourcedb;"
    echo "Importing production database to source..."
    $mysql $sourcedb < $tmpdir/production.sql
  else
    echo "ERROR: $tmpdir/production.sql does not exist."
  fi
  echo "nycc_migrate_import_production_to_source complete."
}

function nycc_migrate_init_target() {
  echo "Clear target database..."
  
  $mysql $targetdb < $scriptsdir/target_init.sql
  
  drush $targetalias watchdog-delete all -y -q 
  
  # TODO: again, test for rules enabled
  truledisabled=`drush $targetalias pml --status=disabled | grep "Rules (rules)"`
  if [ -z "$truledisabled" ]
  then
    echo "Disabling rules..."
    drush $targetalias rules-disable -q rules_display_ride_signup_messages rules_anonymous_user_views_profile rules_ride_join_send_email rules_waitlist_join_send_email_show_message rules_ride_is_submitted rules_ride_is_cancelled rules_ride_withdraw_send_email_show_message rules_ride_is_approved
  fi
  
  echo "Disabling smpt and other target modules during migration..."
  # turn off smtp module and others 
  drush $targetalias dis -y -q  smtp backup_migrate module_filter fpa rules nycc_pic_otw rules_admin rules_scheduler nycc_rides print_pdf

    
  echo "Disabling email traps and membership review..."
  # /admin/config/nycc/nycc_email_trap
  drush $targetalias vset -q nycc_email_trap_exclude_roles notester
  drush $targetalias vset -q nycc_email_trap_enabled 0
  drush $targetalias vset -q nycc_profile_should_redirect_to_membership_review 0
  
  sudo chown -R nyccftp:apache $targetdir/files
  sudo chmod -R 775 $targetdir/files

  # TODO: set the acl to keep files and folders owned by apache  
  
  # Clear out target files directory
  echo "Deleting target files..."
  #sudo rm -R $targetdir/files/
  #sudo mkdir $targetdir/files
  # NOTE: can't use rm -R *
  # NOTE: so not delete .htaccess
  find $targetdir/files -maxdepth 1 -type f -exec sudo rm -f {} \;
  find $targetdir/files -maxdepth 1 -type d -not -name "files" -exec sudo rm -R -f {} \;
  
  echo "nycc_migrate_init_target complete."
}

# RUN MIGRATION sql and drush/php SCRIPTS (using SOURCE TO TARGET)
function nycc_migrate_copy_source_to_target() {
  echo "Copying source to target..."
  # simple updates of base tables
  echo "Copying core tables..."
  $mysqlexec $scriptsdir/role.sql
  $mysqlexec $scriptsdir/users.sql
  $mysqlexec $scriptsdir/profile.sql
  $mysqlexec $scriptsdir/users_roles.sql
  $mysqlexec $scriptsdir/node.sql
  $mysqlexec $scriptsdir/node_revision.sql
  $mysqlexec $scriptsdir/file_managed.sql
  $mysqlexec $scriptsdir/comment.sql
  $mysqlexec $scriptsdir/copy_comments_body.sql
  $mysqlexec $scriptsdir/node_comment_statistics.sql
  $mysqlexec $scriptsdir/url_alias.sql
  $mysqlexec $scriptsdir/history.sql
  $mysqlexec $scriptsdir/field_body.sql
  $mysqlexec $scriptsdir/taxonomy_index.sql
  $mysqlexec $scriptsdir/nycc_user_badge_award.sql
  $mysqlexec $scriptsdir/user_badges_badges.sql
  $mysqlexec $scriptsdir/user_badges_roles.sql
  $mysqlexec $scriptsdir/user_badges_user.sql

  echo "Copying field tables..."
  # content-types page and event fields - multivalued fields
  $fieldcopy --nodelta carousel_order
  $fieldcopy date
  $fieldcopy --kind=fid --notnull image_cache 

  # content-type rides single values
  $fieldcopy --type=rides ride_type ride_select_level ride_speed  ride_spots ride_status ride_signups ride_token ride_dow
  
  # content-type rides single values with formats
  $fieldcopy --type=rides --addcol="field_ride_description_format,5" ride_description 
  $fieldcopy --type=rides --addcol="field_ride_distance_in_miles_format,7" ride_distance_in_miles 
  $fieldcopy --type=rides --addcol="field_ride_speed_format,7" ride_speed 
  $fieldcopy --type=rides --addcol="field_ride_token_format,7" ride_token 
  
  # TODO: ride_timestamp - check for invalid dates?
  $fieldcopy --type=rides ride_timestamp --where="NOT content_type_rides.field_ride_timestamp_value LIKE '0000%'"
  
  $fieldcopy --type=rides --kind=nid ride_cue_sheet 
  $fieldcopy --kind=uid ride_current_riders
  # copy profile nids to uids for now, convert in target cleanup
  $fieldcopy --kind=nid --targetkind=uid --targettype=ride_current_leaders --targetfield=ride_current_leaders ride_leaders
  # handle field renames and simple conversions
  $fieldcopy --type=rides --targetfield=ride_start_location --sourceexp="IFNULL(REPLACE(REPLACE(IFNULL(field_ride_from_value,SUBSTR(field_ride_from_select_value, LOCATE('>',field_ride_from_select_value)+1)),'&#39;','&apos;'),'</a>',''),'TBA')"  --addcol="field_ride_start_location_format,7" ride_start_location

  # content-type rides multis
  $fieldcopy --kind=fid --notnull ride_image
  $fieldcopy --kind=uid ride_waitlist
  $fieldcopy --kind=fid --notnull ride_attachments 
  
  # TODO: new ride fields not populated at this time - skipping - do we need to init?
  # field_data_field_ride_open_signup_days
  # field_data_field_ride_rwgps_link

  # Region
  $fieldcopy --type=region --kind=lid region_location --where="content_type_region.field_region_location_lid > 0"
  
  # Events
  $fieldcopy --nodelta event_category event_spots
  
  # TODO: skip? looks like this may be different from similarly name souce field? 
  # field_data_field_event_view_signups   #boolean

  # Profile
  $fieldcopy --type=profile --entitytype="'profile2'" age_range gender registration_date_import riding_style 
  
  $fieldcopy ride_coordinator
  
  # TODO: file attach?/upload - why?
  # TODO: init new field? field_profile_extra(new)
  
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_address_format,5" address 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_city_format,5" city 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_contact_name_format,5" contact_name 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_country_format,5" country 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_emergency_contact_no_format,5" emergency_contact_no 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_first_name_format,5" first_name 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_profile_last_eny_year_format,5" profile_last_eny_year 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_last_name_format,5" last_name 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_phone_format,5" phone 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_profile_extra_format,5" profile_extra 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_review_last_date_format,5" review_last_date 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_state_format,5" state 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_waiver_last_date_format,5" waiver_last_date 
  $fieldcopy --type=profile --entitytype="'profile2'" --addcol="field_zip_format,5" zip

  $fieldcopy --type=profile --entitytype="'profile2'" --sourceexp="IF(content_type_profile.field_email_list_flag_value='off',0,1)" email_list_flag 
  $fieldcopy --type=profile --entitytype="'profile2'" --sourceexp="IF(content_type_profile.field_publish_email_flag_value='off',0,1)" publish_email_flag 
  $fieldcopy --type=profile --entitytype="'profile2'" --sourceexp="IF(content_type_profile.field_publish_address_flag_value='off',0,1)" publish_address_flag 
  $fieldcopy --type=profile --entitytype="'profile2'" --sourceexp="IF(content_type_profile.field_publish_phone_flag_value='off',0,1)" publish_phone_flag 
  $fieldcopy --type=profile --entitytype="'profile2'" --sourceexp="IF(content_type_profile.field_terms_of_use_value='off',0,1)" terms_of_use 

  # init new fields
  $fieldcopy --type=profile --entitytype="'profile2'" --sourceexp="1" ride_reminders
  $fieldcopy --type=profile --entitytype="'profile2'" --sourceexp="1" ride_rosters
  
  # Cue-Sheets
  $fieldcopy --type="cue_sheet" cuesheet_rating cuesheet_distance cue_sheet_difficulty

  $fieldcopy cuesheet_status2
  $fieldcopy cuesheet_levels
  $fieldcopy --kind=nid cuesheet_region
  $fieldcopy --kind=fid cue_sheet_attachments
  $fieldcopy --notnull --kind=value --targetkind=tid cuesheet_tags

  $fieldcopy --type="cue_sheet" --addcol="field_cuesheet_waypoints_format,5" cuesheet_waypoints
  $fieldcopy --type="cue_sheet" --addcol="field_cue_sheet_author_format,5" cue_sheet_author
  $fieldcopy --type="cue_sheet" --addcol="field_vertical_gain_format,5" vertical_gain
  $fieldcopy --type="cue_sheet" --sourceexp="IF(content_type_cue_sheet.field_cuesheet_signature_route_value='off',0,1)" cuesheet_signature_route 

  # Gear
  $fieldcopy --type=gear --kind=fid gear_image
  $fieldcopy --type=gear --kind=url gear_buy_link

  
  # TODO: special case: files in d6.content_type_cue_sheet.cue_sheet_map_fid, etc
  # TODO: ? field_data_field_cue_sheet_rwgps_link                 ### link
  # TODO: more content-type copies here: obride, others?
  
  # copy files from $source to $target
  #  --exclude="files/files"
  echo "Copying files from source to target..."
  sudo rsync --recursive --exclude="backup_migrate/backup_migrate" --exclude="styles" --exclude="js" --exclude="css" --exclude="imagecache" --exclude="ctools" --exclude="print_pdf" --exclude="imagefield_thumbs" $sourcedir/files $targetdir

  echo "Setting target file permissions..."
  sudo chown -R nyccftp:apache $targetdir/files
  sudo chmod -R 775 $targetdir/files

  echo "nycc_migrate_copy_source_to_target complete."
}

function nycc_migrate_cleanup_source() {
  echo "Cleaning up source..."
  # NOTE: these modules give us errors in drush so we turned them off during migration
  # NOTE: some are a problem on the site too, so leave off?
  echo "Reactivating specific source modules (TODO: NOT!)..."
  # DONE: nycc_ipn
  # $mysql $sourcedb -e"UPDATE system SET status = 1 WHERE system.name IN ('nycc_email', 'rules', 'watchdog_rules', 'logging_alerts');  
  
  # clean up source files folder before rsync
  # clear out ctools?, css? js? print_pdf?
  # delete files/files?
  
  
  echo "nycc_migrate_cleanup_source complete."
}


# RUN MIGRATION "CLEANUP" SCRIPTS ON TARGET
function nycc_migrate_cleanup_target() {
  echo "Cleanup target..."

  echo "Convert passwords..."
  # convert users passwords for use with d7
  drush $targetalias scr $scriptsdir/users-convert-pass.php

  # TODO: extend to handle all managed files
  echo "Convert files..."
  # convert files for d7 use
  drush $targetalias scr $scriptsdir/users-convert-pictures.php --filesdir="$targetdir/files" --subdir=pictures
  
  # factor this out into own operation as it is a long one
  echo "Load/save nodes..." >> $logfile
  # load/save all nodes and users to trigger other modules hooks
  # WARNING: this processes approx 37K records and can take an hour or so if not filtered
  # default is to filter 1=0
  drush $targetalias scr $scriptsdir/node-convert-load-save.php --where="type='forum'"
  
  
  # TODO: create other folders?
  #sudo mkdir -p sites/default/files/backup_migrate
  
  
  echo "Seting files perms..."
  sudo chown -R nyccftp:apache $targetdir/files
  sudo chmod -R 775 $targetdir/files
  
  # TODO: enable when running for real
  echo "Re-enable modules (TODO: NOT smtp!) ..."
  # drush $targetalias en -y -q smtp 
  drush $targetalias en -y -q rules nycc_pic_otw rules_admin rules_scheduler nycc_rides nycc_rides2 print_pdf
  
  echo "Re-enable email and membership review ..."
  # /admin/config/nycc/nycc_email_trap
  drush $targetalias vset -q nycc_email_trap_exclude_roles tester
  drush $targetalias vset -q nycc_email_trap_enabled 1
  drush $targetalias vset -q nycc_profile_should_redirect_to_membership_review 1
  
  
  echo "Re-enable rules (TODO: NOT!) ..."
  #drush $targetalias rules-enable -q rules_display_ride_signup_messages rules_anonymous_user_views_profile rules_ride_join_send_email rules_waitlist_join_send_email_show_message rules_ride_is_submitted rules_ride_is_cancelled rules_ride_withdraw_send_email_show_message rules_ride_is_approved  
  
  $mysql $targetdb < $scriptsdir/target_cleanup.sql
  
  drush $targetalias cc all -q 
  
  # TODO: put message to watchdog
  
  # TODO: set caching and aggregation off in target if not a production deploy
  # /admin/config/development/performance
  
  # clean up target files folder 
  # clear out ctools?, css? js? print_pdf?
  # delete files/files?
    
  
  # ADDITIONAL CLEANUP NOTES/CONSIDERATIONS:
  #
  # 1. remove blocked users? consider any nodes owned by them. other records?
  # 2. remove field records associated with blank (spaces or null or empty string) values in certain cases?
  # 3. remove nodes with no type? (already done in migration copy. seems better suited to remove those early)
  # 4.
  # 5. drush remove obsolete fields and content types?
  # 6. consider leader conversion from nid to uid that has no join (eg, invaid uid?). this is probably not considered in current migration query and left in place
  #
  
  echo "nycc_migrate_cleanup_target complete."
}

# TODO: delete migrate related objects not part of site (eg temp tables)
function nycc_migrate_cleanup_migration() {
  echo "nycc_migrate_cleanup_migration..."
  # TODO: delete $tmpdir/production.sql after successful import
  # TODO: delete $productiontmpdir/production.sql after successful rsync
  # TODO: delete backups from previous migration tests (.sql)
  
  echo "nycc_migrate_cleanup_migration complete."
}

# TODO: output basic counts for comparison
function nycc_migrate_report_target() {
  echo "nycc_migrate_report_target..."
  $mysql -e"SELECT COUNT(*) as users FROM $targetdb.users; SELECT COUNT(*) as nodes FROM $targetdb.node; "
  echo "nycc_migrate_report_target complete."
}

# TODO: output basic counts for comparison
function nycc_migrate_report_source() {
  echo "nycc_migrate_report_source..."
  $mysql -e"SELECT COUNT(*) as users FROM $sourcedb.users; SELECT COUNT(*) as nodes FROM $sourcedb.node; "
  echo "nycc_migrate_report_source complete."
}

# Output current migration status
function nycc_migrate_status() {
  # check for migrate log
  # check for tmp and other folders
  # check for production db
  # check for sourcedb and target db 
  # and various tables 
  # and row counts 
  # last migration step and message (save somewhere?)
  # other important sanity checks/info?
  # report on target and source rules, modules and variables of interest to migration
  # report on smtp status
  
  echo "Status: logfile: `ls -la $logfile`"
}

function nycc_migrate_sync_reference_to_target() {
  # update target from reference site to target site prior to migration
  #
  # goal: site runs and produces no drush migration errors
  #
  #
  # assumes same / similar code base via git

  echo "nycc_migrate_sync_reference_to_target..."
  drush $referencealias cc all

  drush sql-sync $referencealias $targetalias -y

  # Clear out target files directory
  echo "Deleting target files..."
  find $targetdir/files -maxdepth 1 -type f -exec sudo rm -f {} \;
  find $targetdir/files -maxdepth 1 -type d -not -name "files" -exec sudo rm -R -f {} \;
  
  # copy files from reference to $target
  echo "Copying files from reference to target..."
  #sudo rsync --recursive --exclude="files/files" $referencedir/files $targetdir
  sudo rsync --recursive $referencedir/files $targetdir
   
  echo "Setting target file permissions..."
  sudo chown -R nyccftp:apache $targetdir/files
  sudo chmod 775 $targetdir/files
  sudo chmod -R 775 $targetdir/files  

  $mysql $targetdb < $scriptsdir/target_post_reference_sync.sql
  
  drush $targetalias watchdog-delete all -y -q 
  drush $targetalias cc all
  
  echo "nycc_migrate_sync_reference_to_target complete."
}

function show_script_vars() { 
  echo ""
  echo "Script variables:"
  declare -p | grep "\-\- production" | sed -e "s/declare \-\- //"
  echo ""
  declare -p | grep "\-\- source" | sed -e "s/declare \-\- //"
  echo ""
  declare -p | grep "\-\- target" | sed -e "s/declare \-\- //"
  echo ""
  declare -p | grep "\-\- reference" | sed -e "s/declare \-\- //"
  declare -p | grep "\-\- tmpdir" | sed -e "s/declare \-\- //"
  declare -p | grep "\-\- logfile" | sed -e "s/declare \-\- //"
  declare -p | grep "\-\- scriptsdir" | sed -e "s/declare \-\- //"
}

###### END OF MIGRATION FUNCTIONS

###### MAIN - PARSE ARGS AND OPTIONS

# Exit and show help if the command line is empty
[[ ! "$*" ]] && nycc_migrate_usage 1

# Parse command line options
while getopts abcdmprstvwxyz0123456789h\? option
do
    case $option in
        m|1) migration_init=1 ;;
        b|2) backups=1 ;;
        p|3) production_sync=1 ;;
        d|4) import_production_to_source=1 ;;
        c|5) clear_target=1 ;;
        a|6) copy_source_to_target=1 ;;
        w|7) cleanup_source=1 ;;
        x|8) cleanup_target=1 ;;
        y|9) cleanup_migration=1 ;;
        s) migration_status=1 ;;
        r) sync_reference_to_target=1 ;;
        t|0) migration_test=1 ;;
        v) is_verbose=1 ;;
        i) report_source=1 ;;
        j) report_target=1 ;;
        h) nycc_migrate_usage ;;
        \?) nycc_migrate_usage 1 ;;
    esac
done
shift $(($OPTIND - 1));     # take out the option flags

# TODO: output summary of operations with final y/n are you sure
#read -p "Press any key to continue..." -n1 -s
#echo ""

###### MAIN - ORCHESTRATE MIGRATION

if [ -z "$migration_init" ]
then
  echo "Skipped: Migration init (-m or -1)"  >> $logfile
else
  # init migration log
  nycc_migrate_init_migration > $logfile
fi


echo "Logging to $logfile"
echo "Migration steps started at $timestamp" >> $logfile

if [ -z "$backups" ]
then
  echo "Skipped: Backups (-b or -2)"  >> $logfile
else
  nycc_migrate_backup_source_and_target  >> $logfile
fi


if [ -z "$production_sync" ]
then
  echo "Skipped: Production sync (-p or -3)"  >> $logfile
else
  nycc_migrate_export_production >> $logfile
  nycc_migrate_sync_production_to_source >> $logfile
fi


if [ -z "$import_production_to_source" ]
then
  echo "Skipped: Import production to source (-d or -4)"  >> $logfile
else
  nycc_migrate_import_production_to_source >> $logfile
fi


if [ -z "$clear_target" ]
then
  echo "Skipped: Init target (-c or -5)"  >> $logfile
else
  nycc_migrate_init_target >> $logfile
fi


if [ -z "$copy_source_to_target" ]
then
  echo "Skipped: Copy source to target (-a or -6)"  >> $logfile
else
  nycc_migrate_copy_source_to_target >> $logfile
fi


if [ -z "$cleanup_source" ]
then
  echo "Skipped: Cleanup source (-w or -7)"  >> $logfile
else
  nycc_migrate_cleanup_source >> $logfile
fi


if [ -z "$cleanup_target" ]
then
  echo "Skipped: Cleanup target (-x or -8)"  >> $logfile
else
  nycc_migrate_cleanup_target >> $logfile
fi


if [ -z "$cleanup_migration" ]
then
  echo "Skipped: Cleanup migration  (-y or -9)"  >> $logfile
else
  nycc_migrate_cleanup_migration >> $logfile
fi


if [ -z "$report_source" ]
then
  echo "Skipped: Report source (-i)"   >> $logfile
else
  nycc_migrate_report_source >> $logfile
fi


if [ -z "$report_target" ]
then
  echo "Skipped: Report target (-j)"   >> $logfile
else
  nycc_migrate_report_target >> $logfile
fi


if [ -z "$migration_status" ]
then
  # echo "Skipped: Migration status (-s)"
  echo ""
else
  nycc_migrate_status
fi


if [ -z "$sync_reference_to_target" ]
then
  # echo "Skipped: sync_reference_to_target (-r)"
  echo ""
else
  nycc_migrate_sync_reference_to_target
fi


if [ -z "$migration_test" ]
then
#  echo "Skipped: test (-t)"
  echo ""
else
  echo "Starting test run..." | tee --append $logfile
  
  # echo "drush $targetalias scr $scriptsdir/sqlexec.php --sourcedb=$sourcedb" $scriptsdir/test.sql
  # echo "$mysqlexec $scriptsdir/test.sql --sql"
  # echo "Note: no output from test.sql"
  # $mysqlexec $scriptsdir/test.sql --sql
  # echo "Test of mysqlexec test.sql complete."
  echo ""
  show_script_vars | tee --append $logfile
 
  # Gear
  $fieldcopy --type=gear --kind=fid gear_image
  $fieldcopy --type=gear --kind=url gear_buy_link
  
  echo ""
  echo "Test run complete." | tee --append $logfile

fi
 

echo "All migration tasks completed." | tee --append $logfile
