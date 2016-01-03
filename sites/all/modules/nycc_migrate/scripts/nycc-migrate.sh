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
readonly productionfilesdir="/var/www/html/nycc/sites/default/files/"
readonly productiontmpdir="/tmp"
readonly productionssh="/home/markus/.ssh/id_rsa"
readonly sourcealias="@d6Test"
#readonly sourcedb="d6test"
readonly sourcedb="migtest"
#readonly sourcefilesdir="/var/www/html/d6/sites/default/files"
readonly sourcefilesdir="/home/markus/backups/files"
#readonly targetalias="@markusTest"
readonly targetalias="--root=/var/www/html/markus"
readonly targetdb="markus"
readonly targetdir="/var/www/html/markus/sites/default/files"
readonly logfile="$tmpdir/migrate.log"
readonly timestamp="`date +%Y-%m-%d_%H-%M`"

# command aliases
readonly mysql='mysql -uroot -pXt2792b8cf'
readonly mysqldump='mysqldump -uroot -pXt2792b8cf'
readonly fieldcopy="drush $targetalias scr $scriptsdir/field_copy.php --sourcedb=$sourcedb"


# TODO: check all folders and settings for validity
function nycc_migrate_init_migration() {
  echo "Migration init..."
  mkdir -p $tmpdir
  echo "NYCC Migration - $timestamp"
  # TODO: display config/options
  echo "$@"
}

# PREPARE: EXPORT PRODUCTION DATABASE, RYSYNC DATABASE AND FILES TO TARGET
function nycc_migrate_backup() {
  echo "Making backup of source database..."
  $mysqldump $sourcedb > $tmpdir/$sourcedb-$timestamp.sql

  echo "Making backup of target database..."
  $mysqldump $targetdb > $tmpdir/$targetdb-$timestamp.sql
}

function nycc_migrate_export_production() {
  echo "Exporting production database..."
  ssh $productionuser "$mysqldump $productiondb > $productiontmpdir/production.sql"
  
  echo "Rsyncing production database export..."
  sudo rsync -z -e "ssh -i $productionssh" $productionuser:$productiontmpdir/production.sql $tmpdir
}

function nycc_migrate_sync_production_to_source() {
  echo "Rsyncing production files (update)..."
  sudo rsync -azu -e "ssh -i $productionssh" --exclude="backup_migrate" --exclude="js" --exclude="css" --exclude="imagecache" $productionuser:$productionfilesdir $sourcefilesdir

  sudo chown -R nyccftp:apache $sourcefilesdir
  sudo chown -R 775 $sourcefilesdir

  # these modules give us errors in drush
  $mysql $sourcedb -e"UPDATE system SET status = 0 WHERE system.name IN ('nycc_email', 'rules', 'watchdog_rules', 'logging_alerts', 'nycc_ipn');"
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
}

function nycc_migrate_init_target() {
  echo "Clear target database..."

  $mysql $targetdb < $scriptsdir/target_init.sql
  
  # TODO: also clear target file folders? optional
  echo "Deleting target files..."
  rm -R $targetdir
 
  # turn off smtp module
  drush $targetalias dis -y smtp
  
  # http://markus.test.nycc.org/admin/config/nycc/nycc_email_trap
  drush $targetalias vset nycc_email_trap_exclude_roles notester
  drush $targetalias vset nycc_email_trap_enabled 0
  
  
  # disable rules
  drush $targetalias rules-disable rules_display_ride_signup_messages rules_anonymous_user_views_profile rules_ride_join_send_email rules_waitlist_join_send_email_show_message rules_ride_is_submitted rules_ride_is_cancelled rules_ride_withdraw_send_email_show_message rules_ride_is_approved
 }

# RUN MIGRATION "COPY" SCRIPTS FROM SOURCE TO TARGET
function nycc_migrate_copy_source_to_target() {
  echo "Copying source to target..."
  # simple updates of base tables
  $mysql $targetdb < $scriptsdir/role.sql
  $mysql $targetdb < $scriptsdir/users.sql
  $mysql $targetdb < $scriptsdir/users_roles.sql
  $mysql $targetdb < $scriptsdir/node.sql
  $mysql $targetdb < $scriptsdir/node_revision.sql
  $mysql $targetdb < $scriptsdir/file_managed.sql

  # content-type page - multivalued fields
  $fieldcopy carousel_order
  $fieldcopy date
  $fieldcopy --kind=fid image_cache 

  # content-type rides single values
  $fieldcopy --type=rides ride_description ride_type ride_select_level ride_speed ride_distance_in_miles ride_signups ride_spots ride_status ride_signups ride_token ride_timestamp ride_dow
  
  # TODO: ride_timestamp - check for invalid dates?
  $fieldcopy --type=rides ride_timestamp --where="not content_type_rides.field_ride_timestamp_value like '0000%'"
  
  
  # non value fields in rides content table
  $fieldcopy --type=rides --kind=nid ride_cue_sheet 
  
  # omit type for simple ref copies
  $fieldcopy --kind=uid ride_current_riders   
  $fieldcopy --kind=nid ride_leaders
  
  # handle field renames and simple conversions
  $fieldcopy --type=rides --targetfield=ride_start_location --sourceexp="IFNULL(REPLACE(REPLACE(IFNULL(field_ride_from_value,SUBSTR(field_ride_from_select_value, LOCATE('>',field_ride_from_select_value)+1)),'&#39;','&apos;'),'</a>',''),'TBA')" ride_start_location

  # content-type rides multis
  $fieldcopy --kind=fid ride_image
  $fieldcopy --kind=uid ride_waitlist
  $fieldcopy --kind=fid ride_attachments 

  # new ride fields not populated at this time
  # field_data_field_ride_open_signup_days
  # field_data_field_ride_rwgps_link

  # Events
  # field_data_field_event_category
  # field_data_field_event_spots
  # field_data_field_event_view_signups
  
  # cue-sheets
  # field_data_field_cue_sheet_attachments
  # field_data_field_cue_sheet_author
  # field_data_field_cue_sheet_difficulty
  # field_data_field_cue_sheet_rwgps_link
  # field_data_field_cuesheet_distance
  # field_data_field_cuesheet_levels
  # field_data_field_cuesheet_rating
  # field_data_field_cuesheet_region
  # field_data_field_cuesheet_signature_route
  # field_data_field_cuesheet_status2
  # field_data_field_cuesheet_tags
  # field_data_field_cuesheet_waypoints
  # field_data_field_vertical_gain?
  
  # region
  # field_data_field_region_location
  
  # profile
  # field_data_field_address
  # field_data_field_age_range
  # field_data_field_city
  # field_data_field_contact_name
  # field_data_field_country
  # field_data_field_email_list_flag
  # field_data_field_emergency_contact_no
  # field_data_field_first_name
  # field_data_field_gender
  # field_data_field_last_name
  # field_data_field_phone
  # field_data_field_profile_extra
  # field_data_field_profile_last_eny_year
  # field_data_field_publish_address_flag
  # field_data_field_publish_email_flag
  # field_data_field_publish_phone_flag
  # field_data_field_registration_date_import
  # field_data_field_review_last_date
  # field_data_field_state
  # field_data_field_terms_of_use
  # field_data_field_waiver_last_date
  # field_data_field_zip  
  

  
  # TODO: more content-type copies here: mb (forumn topic)?, obride?
  # ALSO: comments (forum and ?)
  
  
  # copy files from $source to $target
  echo "Copying files from source to target..."
  rsync --recursive --update --quiet --delete $sourcedir $targetdir
}

# RUN MIGRATION "CLEANUP" SCRIPTS ON TARGET
function nycc_migrate_cleanup_target() {
  echo "Cleanup target..."

  # cleanup rides issues such as description formats
  echo "Cleaning up rides descriptions..."
  $mysql $targetdb < $scriptsdir/node-rides-formats.sql

  echo "Convert passwords..."
  # convert users passwords for use with d7
  drush $targetalias scr $scriptsdir/users-convert-pass.php

  echo "Convert files..."
  # convert files for d7 use
  drush $targetalias scr $scriptsdir/users-convert-pictures.php
  
  echo "Load/save nodes..." >> $logfile
  # load/save all nodes and users to trigger other modules hooks?
  drush $targetalias scr $scriptsdir/node-convert-load-save.php

  # TODO: Additional processing:
  # check that profile2 pid's work as expected. what is test for this?
  
  echo "Seting files perms..."
  sudo chown -R nyccftp:apache $targetdir
  sudo chmod -R 775 $targetdir
  
  echo "Re-enable modules..."
  drush $targetalias en -y smtp
  
  echo "Re-enable email..."
  # http://markus.test.nycc.org/admin/config/nycc/nycc_email_trap
  drush $targetalias vset nycc_email_trap_exclude_roles tester
  drush $targetalias vset nycc_email_trap_enabled 1
  
  
  echo "Re-enable rules..."
  drush $targetalias rules-enable rules_display_ride_signup_messages rules_anonymous_user_views_profile rules_ride_join_send_email rules_waitlist_join_send_email_show_message rules_ride_is_submitted rules_ride_is_cancelled rules_ride_withdraw_send_email_show_message rules_ride_is_approved  
  
}

# TODO: delete migrate related objects not part of site (eg temp tables)
function nycc_migrate_cleanup_migration() {
  echo "nycc_migrate_cleanup_migration - noop"
  # TODO: delete $tmpdir/production.sql after successful import
  # TODO: delete $productiontmpdir/production.sql after successful rsync
}

# TODO: output basic counts for comparison
function nycc_migrate_report_target() {
  echo "nycc_migrate_report_target"
  $mysql -e"SELECT COUNT(*) as users FROM $targetdb.users; SELECT COUNT(*) as nodes FROM $targetdb.node; "
}

# TODO: output basic counts for comparison
function nycc_migrate_report_source() {
  echo "nycc_migrate_report_source"
  $mysql -e"SELECT COUNT(*) as users FROM $sourcedb.users; SELECT COUNT(*) as nodes FROM $sourcedb.node; "
}

function nycc_migrate_usage () {
    echo "
Usage: $(basename $0) [-options] [site] [user]
    -a -6           copy source to target
    -b -2           backups (source and target)
    -c -5           clear out target database and files before migration
    -d -4           import production to source
    -m -1           migration init
    -p -3           production export and sync
    -r              target backup (not implemented yet)
    -s              source backup (not implemented yet)
    -t -0           test
    -v              verbose (nothing extra yet)
    -w -7           cleanup target
    -x -8           cleanup migration
    -y              source report
    -z              target report
    -h              this usage help text
    site            target site
Migrate NYCC Drupal 6 to 7
Example:
    $(basename $0) -p d7test"
    exit ${1:-0}
}

function ask_if_empty () {
    local value="$1"
    local default="$2"
    local message="$3"
    local options="$4"  # pass "-s" for passwords
    if [[ -z "$value" ]]; then
        read $options -p "$message [$default] " value
    fi
    value=$(echo ${value:-$default})
    echo "$value"
}

# Exit and show help if the command line is empty
[[ ! "$*" ]] && nycc_migrate_usage 1

# Initialise options
n_value="value if option is missing"

# Parse command line options
while getopts abcdmprstvwxyz0123456789h\? option
do
    case $option in
        a|6) copy_source_to_target=1 ;;
        b|2) backups=1 ;;
        c|5) clear_target=1 ;;
        d|4) import_production_to_source=1 ;;
        m|1) migration_init=1 ;;
        p|3) production_sync=1 ;;
        r) target_backup=1 ;;
        s) source_backup=1 ;;
        t|0) test=1 ;;
        v) is_verbose=1 ;;
        w|7) cleanup_target=1 ;;
        x|8) cleanup_migration=1 ;;
        y) report_source=1 ;;
        z) report_target=1 ;;
        h) nycc_migrate_usage ;;
        \?) nycc_migrate_usage 1 ;;
    esac
done
shift $(($OPTIND - 1));     # take out the option flags

# Validate input parameters
#parameter=$(ask_if_empty "$1" "d7" "Enter the target:")
#echo Target is $parameter

# TODO: output summary of operations with final y/n are you sure
#read -p "Press any key to continue..." -n1 -s
#echo ""



# MAIN


if [ -z "$migration_init" ]
then
  echo "Skipped: Migration init (-m)"
else
  nycc_migrate_init_migration > $logfile
fi


if [ -z "$backups" ]
then
  echo "Skipped: Backups (-b)"
else
  nycc_migrate_backup  >> $logfile
fi


if [ -z "$production_sync" ]
then
  echo "Skipped: Production sync (-p)"
else
  nycc_migrate_export_production >> $logfile
  nycc_migrate_sync_production_to_source >> $logfile
fi


if [ -z "$import_production_to_source" ]
then
  echo "Skipped: Import production to source (-d)"
else
  nycc_migrate_import_production_to_source >> $logfile
fi


if [ -z "$clear_target" ]
then
  echo "Skipped: Init target (-c)"
else
  nycc_migrate_init_target >> $logfile
fi


if [ -z "$copy_source_to_target" ]
then
  echo "Skipped: Copy source to target (-a)"
else
  nycc_migrate_copy_source_to_target >> $logfile
fi


if [ -z "$cleanup_target" ]
then
  echo "Skipped: Cleanup target (-w)"
else
  nycc_migrate_cleanup_target >> $logfile
fi


if [ -z "$cleanup_migration" ]
then
  echo "Skipped: Cleanup migration (-x)"
else
  nycc_migrate_cleanup_migration >> $logfile
fi


if [ -z "$report_source" ]
then
  echo "Skipped: Report source (-y)"
else
  nycc_migrate_report_source >> $logfile
fi


if [ -z "$report_target" ]
then
  echo "Skipped: Report target (-z)"
else
  nycc_migrate_report_target >> $logfile
fi


if [ -z "$test" ]
then
#  echo "Skipped: test (-t)"
  echo "";
else
  echo "Test run."
  
   echo "Cleaning up ride descriptions"
  drush $targetalias vset nycc_email_trap_exclude_roles notester
  drush $targetalias vset nycc_email_trap_enabled 0
  


fi
 


echo "Migration tasks completed."  >> $logfile
