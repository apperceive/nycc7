#!/bin/bash

####################################################
#
# NYCC migration from Drupal 6 to 7
#
# copy database and file from production to source
# perform migration from source to target
####################################################


readonly BASEDIR=$(cd "$(dirname "$0")" && pwd) # where the script is located
readonly CALLDIR=$(pwd)                         # where it was called from
#readonly STATUS_SUCCESS=0                       # exit status for commands

# GLOBALS - edit as needed for user and folders
# TODO: factor out $user and $subdomain (both markus here)
# NOTES: tmpdir will be created if need be
readonly scriptsdir="/var/www/html/markus/sites/all/modules/nycc_migrate/scripts/migrate"
readonly tmpdir="/home/markus/backups"

readonly productiondb="nycc"
readonly productionuser="markus@nycc.org"
readonly productionfilesdir="/var/www/html/nycc/sites/default"
readonly productiontmpdir="/tmp"
readonly productionssh="/home/markus/.ssh/id_rsa"

readonly sourcealias="@d6Test"
#readonly sourcedb="d6test"
readonly sourcedb="migtest"
# note: all commands must append /files to $sourcedir for safety
#readonly sourcedir="/var/www/html/d6/sites/default"
readonly sourcedir="/home/markus/backups"

#readonly targetalias="@markusTest"
readonly targetalias="--root=/var/www/html/markus"
readonly targetdb="markus"
# note: all commands must append /files to $targetdir for safety
readonly targetdir="/var/www/html/markus/sites/default"

readonly logfile="$tmpdir/migrate.log"
readonly timestamp="`date +%Y-%m-%d_%H-%M`"

# command aliases
readonly mysql='mysql -uroot -pXt2792b8cf'
readonly mysqldump='mysqldump -uroot -pXt2792b8cf'
readonly mysqlexec="drush $targetalias scr $scriptsdir/sqlexec.php --sourcedb=$sourcedb"
readonly fieldcopy="drush $targetalias scr $scriptsdir/field_copy.php --sourcedb=$sourcedb"

# USAGE - HELP

function nycc_migrate_usage () {
    echo "
Usage: $(basename $0) [-options] [site]
    -t -0           run test script
    -m -1           migration init
    -b -2           backups (source and target)
    -p -3           production export and sync
    -d -4           import production to source
    -c -5           clear out target database and files before migration
    -a -6           copy source to target
    -w -7           cleanup source
    -x -8           cleanup target
    -y -9           cleanup migration (and source)
    -r              target backup (not implemented yet, use -b)
    -s              source backup (not implemented yet, use -b)
    -v              verbose (nothing extra yet)
    -i              source report
    -i              target report
    -h              this usage help text
    site            target site
Migrate NYCC Drupal 6 to 7
Example:
    $(basename $0) -p d7test"
    exit ${1:-0}
}

# MIGRATION FUNCTIONS

# INITIALIZE MIGRATION
# TODO: sanity check all folders and settings, issue warnings if need be
# NOTE: this is only for migration objects, not source or target - assume no backup yet
function nycc_migrate_init_migration() {
  echo "Migration init..."
  mkdir -p $tmpdir
  echo "NYCC Migration - $timestamp"
  # TODO: display config/options
  # echo "$@"
  echo "nycc_migrate_init_migration complete."
}

# EXPORT PRODUCTION DATABASE, RYSYNC DATABASE AND FILES TO TARGET
function nycc_migrate_backup_source_and_target() {
  echo "Making backup of source database..."
  $mysqldump $sourcedb > $tmpdir/$sourcedb-$timestamp.sql

  echo "Making backup of target database..."
  $mysqldump $targetdb > $tmpdir/$targetdb-$timestamp.sql
  
  # TODO: backup sourcedirs and targetdir's
  echo "nycc_migrate_backup_source_and_target complete."
}

function nycc_migrate_export_production() {
  echo "Exporting production database..."
  ssh $productionuser "$mysqldump $productiondb > $productiontmpdir/production.sql"
  
  echo "Rsyncing production database export..."
  sudo rsync -z -e "ssh -i $productionssh" $productionuser:$productiontmpdir/production.sql $tmpdir
  
  echo "nycc_migrate_export_production complete."
}

function nycc_migrate_sync_production_to_source() {
  echo "Rsyncing production files to source (update)..."
  sudo rsync -azu -e "ssh -i $productionssh" --exclude="backup_migrate/backup_migrate" --exclude="styles" --exclude="js" --exclude="css" --exclude="imagecache" --exclude="imagefield_thumbs" $productionuser:$productionfilesdir/files/ $sourcedir/files

  # NOTE: these modules give us errors in drush so turn them off
  # NOTE: some are a problem on the site too, so leave off?
  $mysql $sourcedb -e"UPDATE system SET status = 0 WHERE system.name IN ('nycc_email', 'rules', 'watchdog_rules', 'logging_alerts', 'nycc_ipn');"

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

  # $mysql $targetdb < $scriptsdir/target_init.sql
  # drush $targetalias scr $scriptsdir/sqlexec.php --sourcesb=$sourcedb $scriptsdir/target_init.sql
  $mysql $targetdb < $scriptsdir/target_init.sql
  
  echo "Disabling smpt and other target modules during migration..."
  # turn off smtp module and others 
  drush $targetalias dis -y -q smtp backup_migrate module_filter fpa rules nycc_pic_otw rules_admin rules_scheduler

    
  echo "Disabling email traps..."
  # http://markus.test.nycc.org/admin/config/nycc/nycc_email_trap
  drush $targetalias -q vset nycc_email_trap_exclude_roles notester
  drush $targetalias -q vset nycc_email_trap_enabled 0
  
  # disable rules
  #echo "Disabling rules..."
  #drush $targetalias -q rules-disable rules_display_ride_signup_messages rules_anonymous_user_views_profile rules_ride_join_send_email rules_waitlist_join_send_email_show_message rules_ride_is_submitted rules_ride_is_cancelled rules_ride_withdraw_send_email_show_message rules_ride_is_approved
  
  # Clear out target files directory
  echo "Deleting target files..."
  sudo rm -R $targetdir/files/
  sudo mkdir $targetdir/files
  sudo chown -R nyccftp:apache $targetdir/files
  sudo chown -R 1775 $targetdir/files

  echo "nycc_migrate_init_target complete."
}

# RUN MIGRATION sql and drush/php SCRIPTS (using SOURCE TO TARGET)
function nycc_migrate_copy_source_to_target() {
  echo "Copying source to target..."
  # simple updates of base tables
  echo "Copying core tables..."
  $mysqlexec $scriptsdir/role.sql
  $mysqlexec $scriptsdir/users.sql
  $mysqlexec $scriptsdir/users_roles.sql
  $mysqlexec $scriptsdir/node.sql
  $mysqlexec $scriptsdir/node_revision.sql
  $mysqlexec $scriptsdir/file_managed.sql
  $mysqlexec $scriptsdir/comments.sql
  $mysqlexec $scriptsdir/copy_comments_body.sql

  echo "Copying field tables..."
  # content-types page and event fields - multivalued fields
  $fieldcopy carousel_order
  $fieldcopy date
  $fieldcopy --kind=fid image_cache 

  # content-type rides single values
  $fieldcopy --type=rides ride_type ride_select_level ride_speed  ride_spots ride_status ride_signups ride_token ride_dow
  
  # content-type rides single values with formats
  $fieldcopy --type=rides --addcol="field_ride_description_format,5" ride_description 
  $fieldcopy --type=rides --addcol="field_ride_distance_in_miles_format,7" ride_distance_in_miles 
  $fieldcopy --type=rides --addcol="field_ride_speed_format,7" ride_speed 
  $fieldcopy --type=rides --addcol="field_ride_token_format,7" ride_token 
  
  # TODO: ride_timestamp - check for invalid dates?
  $fieldcopy --type=rides ride_timestamp --where="NOT content_type_rides.field_ride_timestamp_value LIKE '0000%'"
  
  
  # non value fields in rides content table
  $fieldcopy --type=rides --kind=nid ride_cue_sheet 
  
  # omit type for simple ref copies
  $fieldcopy --kind=uid ride_current_riders   
  $fieldcopy --kind=nid ride_leaders
  
  # handle field renames and simple conversions
  $fieldcopy --type=rides --targetfield=ride_start_location --sourceexp="IFNULL(REPLACE(REPLACE(IFNULL(field_ride_from_value,SUBSTR(field_ride_from_select_value, LOCATE('>',field_ride_from_select_value)+1)),'&#39;','&apos;'),'</a>',''),'TBA')"  --addcol="field_ride_start_location_format,7" ride_start_location

  # content-type rides multis
  $fieldcopy --kind=fid ride_image
  $fieldcopy --kind=uid ride_waitlist
  $fieldcopy --kind=fid ride_attachments 
  
  # TODO: new ride fields not populated at this time - skipping - do we need to init?
  # field_data_field_ride_open_signup_days
  # field_data_field_ride_rwgps_link

  # Region
  $fieldcopy --type=region --kind=lid region_location 
  
  # Events
  $fieldcopy event_category event_spots
  
  # TODO: skip? looks like this may be different from similarly name souce field? 
  # field_data_field_event_view_signups   #boolean

  # Profile
  $fieldcopy --type=profile age_range gender registration_date_import
  
  $fieldcopy --type=profile --addcol="field_city_format,5" city 
  $fieldcopy --type=profile --addcol="field_contact_name_format,5" contact_name 
  $fieldcopy --type=profile --addcol="field_country_format,5" country 
  $fieldcopy --type=profile --addcol="field_emergency_contact_no_format,5" emergency_contact_no 
  $fieldcopy --type=profile --addcol="field_first_name_format,5" first_name 
  $fieldcopy --type=profile --addcol="field_profile_last_eny_year_format,5" profile_last_eny_year 
  $fieldcopy --type=profile --addcol="field_last_name_format,5" last_name 
  $fieldcopy --type=profile --addcol="field_phone_format,5" phone 
  $fieldcopy --type=profile --addcol="field_profile_extra_format,5" profile_extra 
  $fieldcopy --type=profile --addcol="field_review_last_date_format,5" review_last_date 
  $fieldcopy --type=profile --addcol="field_state_format,5" state 
  $fieldcopy --type=profile --addcol="field_waiver_last_date_format,5" waiver_last_date 
  $fieldcopy --type=profile --addcol="field_zip_format,5" zip

  $fieldcopy --type=profile --sourceexp="IF(content_type_profile.field_email_list_flag_value='off',0,1)" email_list_flag 
  $fieldcopy --type=profile --sourceexp="IF(content_type_profile.field_publish_email_flag_value='off',0,1)" publish_email_flag 
  $fieldcopy --type=profile --sourceexp="IF(content_type_profile.field_publish_address_flag_value='off',0,1)" publish_address_flag 
  $fieldcopy --type=profile --sourceexp="IF(content_type_profile.field_publish_phone_flag_value='off',0,1)" publish_phone_flag 
  $fieldcopy --type=profile --sourceexp="IF(content_type_profile.field_terms_of_use_value='off',0,1)" terms_of_use 
  
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
  
  # Cue-sheet body =:o
  # TODO: field_body_summary?
  $fieldcopy --type="node_revisions" --addcol="body_format,5" --targettable="field_data_body" --targetfield="body_value" --nosuffix --noprefix --sourceexp="body" --where="node.type='cue-sheet'" body
  
  
  # TODO: special case: files in d6.content_type_cue_sheet.cue_sheet_map_fid, etc
  # TODO: ? field_data_field_cue_sheet_rwgps_link                 ### link
  # TODO: more content-type copies here: obride, others?
  
  # copy files from $source to $target
  echo "Copying files from source to target..."
  sudo rsync --recursive --update --quiet --delete $sourcedir/files/ $targetdir/files

  echo "Setting target file permissions..."
  sudo chown -R nyccftp:apache $targetdir/files
  sudo chown -R 1775 $targetdir/files

  echo "nycc_migrate_copy_source_to_target complete."
}

function nycc_migrate_cleanup_source() {
  echo "Cleaning up source - no-op..."
  # NOTE: these modules give us errors in drush so we turned them off during migration
  # NOTE: some are a problem on the site too, so leave off?
  # $mysql $sourcedb -e"UPDATE system SET status = 1 WHERE system.name IN ('nycc_email', 'rules', 'watchdog_rules', 'logging_alerts', 'nycc_ipn');  
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
  # load/save all nodes and users to trigger other modules hooks?
  ####################drush $targetalias scr $scriptsdir/node-convert-load-save.php

  # TODO: Additional processing:
  # check that profile2 pid's work as expected. what is test for this?
  
  echo "Seting files perms..."
  sudo chown -R nyccftp:apache $targetdir/files
  sudo chmod -R 775 $targetdir/files
  
  # TODO: enable when running for real
  echo "Re-enable modules (NOT!) ..."
  #drush $targetalias en -y -q smtp rules, nycc_pic_otw, rules_admin, rules_scheduler
  
  echo "Re-enable email (NOT!)..."
  # http://markus.test.nycc.org/admin/config/nycc/nycc_email_trap
  #drush $targetalias -q vset nycc_email_trap_exclude_roles tester
  #drush $targetalias -q vset nycc_email_trap_enabled 1
  
  
  echo "Re-enable rules (NOT!) ..."
  #drush $targetalias -q rules-enable rules_display_ride_signup_messages rules_anonymous_user_views_profile rules_ride_join_send_email rules_waitlist_join_send_email_show_message rules_ride_is_submitted rules_ride_is_cancelled rules_ride_withdraw_send_email_show_message rules_ride_is_approved  
  
  drush -q $targetalias cc all
  
  echo "nycc_migrate_cleanup_target complete."
}

# TODO: delete migrate related objects not part of site (eg temp tables)
function nycc_migrate_cleanup_migration() {
  echo "nycc_migrate_cleanup_migration - no-op..."
  # TODO: delete $tmpdir/production.sql after successful import
  # TODO: delete $productiontmpdir/production.sql after successful rsync
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

###### END OF MIGRATION FUNCTIONS

###### MAIN - PARSE ARGS AND OPTIONS

# Exit and show help if the command line is empty
[[ ! "$*" ]] && nycc_migrate_usage 1

# Parse command line options
while getopts abcdmprstvwxyz0123456789h\? option
do
    case $option in
        t|0) migration_test=1 ;;
        m|1) migration_init=1 ;;
        b|2) backups=1 ;;
        p|3) production_sync=1 ;;
        d|4) import_production_to_source=1 ;;
        c|5) clear_target=1 ;;
        a|6) copy_source_to_target=1 ;;
        w|7) cleanup_source=1 ;;
        x|8) cleanup_target=1 ;;
        y|9) cleanup_migration=1 ;;
        r) target_backup=1 ;;
        s) source_backup=1 ;;
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
  echo "Skipped: Migration init (-m)"  >> $logfile
else
  nycc_migrate_init_migration > $logfile
fi


echo "Logging to $logfile"
echo "Migration steps started at $timestamp" >> $logfile

if [ -z "$backups" ]
then
  echo "Skipped: Backups (-b)"  >> $logfile
else
  nycc_migrate_backup_source_and_target  >> $logfile
fi


if [ -z "$production_sync" ]
then
  echo "Skipped: Production sync (-p)"  >> $logfile
else
  nycc_migrate_export_production >> $logfile
  nycc_migrate_sync_production_to_source >> $logfile
fi


if [ -z "$import_production_to_source" ]
then
  echo "Skipped: Import production to source (-d)"  >> $logfile
else
  nycc_migrate_import_production_to_source >> $logfile
fi


if [ -z "$clear_target" ]
then
  echo "Skipped: Init target (-c)"  >> $logfile
else
  nycc_migrate_init_target >> $logfile
fi


if [ -z "$copy_source_to_target" ]
then
  echo "Skipped: Copy source to target (-a)"  >> $logfile
else
  nycc_migrate_copy_source_to_target >> $logfile
fi


if [ -z "$cleanup_source" ]
then
  echo "Skipped: Cleanup source (-w)"  >> $logfile
else
  nycc_migrate_cleanup_source >> $logfile
fi


if [ -z "$cleanup_target" ]
then
  echo "Skipped: Cleanup target (-x)"  >> $logfile
else
  nycc_migrate_cleanup_target >> $logfile
fi


if [ -z "$cleanup_migration" ]
then
  echo "Skipped: Cleanup migration  (-y)"  >> $logfile
else
  nycc_migrate_cleanup_migration >> $logfile
fi


if [ -z "$report_source" ]
then
  echo "Skipped: Report source (-y)"   >> $logfile
else
  nycc_migrate_report_source >> $logfile
fi


if [ -z "$report_target" ]
then
  echo "Skipped: Report target (-z)"   >> $logfile
else
  nycc_migrate_report_target >> $logfile
fi


if [ -z "$migration_test" ]
then
#  echo "Skipped: test (-t)"
  echo "";
else
  echo "Test run..." >> $logfile
  
  # echo "Copy events"
  # $fieldcopy event_category event_spots
  
  # drush $targetalias scr $scriptsdir/test.php --test=123 --test=456
  # $fieldcopy --type=profile --addcol="field_city_format,5" city 
  
  # drush $targetalias scr $scriptsdir/sqlexec.php --sql --targetdb=$targetdb --sourcedb=$sourcedb $scriptsdir/role.sql
  # drush $targetalias scr $scriptsdir/sqlexec.php --sql --sourcesb=$sourcedb $scriptsdir/role.sql
  echo "drush $targetalias scr $scriptsdir/sqlexec.php --sourcedb=$sourcedb" $scriptsdir/test.sql
  echo "Note: no output from test.sql"
  $mysqlexec $scriptsdir/test.sql    
  echo "Test of mysqlexec test.sql complete."
 
  
  #$fieldcopy --sql --type="node_revisions" --addcol="body_format,5" --targettable="field_data_body" --targetfield="body_value" --nosuffix --noprefix --sourceexp="body" --where="node.type='cue-sheet'" body
 
  $fieldcopy --notnull --kind=value --targetkind=tid cuesheet_tags
  
  
  echo "Test complete."

fi
 

echo "Migration tasks completed."  >> $logfile
