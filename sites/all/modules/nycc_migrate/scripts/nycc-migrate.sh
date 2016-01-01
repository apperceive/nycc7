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
#readonly sourcedb="d6test"
readonly sourcedb="migtest"
export sourcedb
#readonly sourcefilesdir="/var/www/html/d6/sites/default/files"
readonly sourcefilesdir="/home/markus/backups/files"
#readonly targetalias="@markusTest"
readonly targetalias="--root=/var/www/html/markus"
readonly targetdb="markus"
readonly targetdir="/var/www/html/markus/sites/default/files"


# TODO: check all folders and settings for validity
function nycc_migrate_init_migration() {
  echo "Migration init..."
  mkdir -p $tmpdir
}

# PREPARE: EXPORT PRODUCTION DATABASE, RYSYNC DATABASE AND FILES TO TARGET
function nycc_migrate_backup() {
  echo "Making backup of source database..."
  mysqldump -uroot -pXt2792b8cf $sourcedb > $tmpdir/$sourcedb-`date +%Y-%m-%d_%H-%M`.sql

  echo "Making backup of target database..."
  mysqldump -uroot -pXt2792b8cf $targetdb > $tmpdir/$targetdb-`date +%Y-%m-%d_%H-%M`.sql
}

function nycc_migrate_export_production() {
  echo "Exporting production database..."
  ssh $productionuser "mysqldump -uroot -pXt2792b8cf $productiondb > $productiontmpdir/production.sql"
  
  echo "Rsyncing production database export..."
  sudo rsync -z -e "ssh -i $productionssh" $productionuser:$productiontmpdir/production.sql $tmpdir
}

function nycc_migrate_sync_production_to_source() {
  echo "Rsyncing production files (update)..."
  sudo rsync -azu -e "ssh -i $productionssh" --exclude="backup_migrate" --exclude="js" --exclude="css" --exclude="imagecache" $productionuser:$productionfilesdir $sourcefilesdir

  sudo chown -R nyccftp:apache $sourcefilesdir
  sudo chown -R 775 $sourcefilesdir

  # these modules give us errors in drush
  mysql -uroot -pXt2792b8cf $sourcedb -e"UPDATE system SET status = 0 WHERE system.name IN ('nycc_email', 'rules', 'watchdog_rules', 'logging_alerts', 'nycc_ipn');"
}

# IMPORT PRODUCTION TO SOURCE DATABASE
function nycc_migrate_import_production_to_source() {
  # CHECK: production.sql file exists in $tmpdir
  if [ -e $tmpdir/production.sql ] 
  then
    echo "Clear source database..."
    mysql -uroot -pXt2792b8cf -e"DROP DATABASE IF EXISTS $sourcedb; CREATE DATABASE $sourcedb;"
    echo "Importing production database to source..."
    mysql -uroot -pXt2792b8cf $sourcedb < $tmpdir/production.sql
  else
    echo "ERROR: $tmpdir/production.sql does not exist."
  fi
}

# TODO: also clear target file folders? optional
function nycc_migrate_clear_target() {
  echo "Clear target database..."
  #TODO: TRUNCATE SPECIFIC TABLES (or use standard d7 install db export?)
  #mysql -uroot -pXt2792b8cf -e"DROP DATABASE IF EXISTS $targetdb; CREATE DATABASE $targetdb;"
  #TODO: delete target files
}

# RUN MIGRATION "COPY" SCRIPTS FROM SOURCE TO TARGET
function nycc_migrate_copy_source_to_target() {
  echo "Copying source to target..."
  # simple updates of base tables
  mysql -uroot -pXt2792b8cf $targetdb < $scriptsdir/role.sql
  mysql -uroot -pXt2792b8cf $targetdb < $scriptsdir/users.sql
  mysql -uroot -pXt2792b8cf $targetdb < $scriptsdir/users_roles.sql
  mysql -uroot -pXt2792b8cf $targetdb < $scriptsdir/node.sql
  mysql -uroot -pXt2792b8cf $targetdb < $scriptsdir/node_revision.sql
  mysql -uroot -pXt2792b8cf $targetdb < $scriptsdir/file_managed.sql

  # content-type page - multivalued fields
  drush $targetalias scr $scriptsdir/field_copy.php carousel_order
  drush $targetalias scr $scriptsdir/field_copy.php date
  drush $targetalias scr $scriptsdir/field_copy.php --kind=fid image_cache 

  # content-type rides single values
  drush $targetalias scr $scriptsdir/field_copy.php --type=rides ride_description ride_type ride_select_level ride_speed ride_type ride_distance_in_miles ride_signups ride_spots ride_status ride_signups ride_token ride_timestamp
  
  # non value fields
  drush $targetalias scr $scriptsdir/field_copy.php --type=rides --kind=nid ride_cue_sheet 
  drush $targetalias scr $scriptsdir/field_copy.php --type=rides --kind=nid --targetkind=uid --targetfield=ride_current_leaders ride_leaders   
  drush $targetalias scr $scriptsdir/field_copy.php --type=rides --kind=uid ride_current_riders   
  
  # handle field renames
  #drush $targetalias scr $scriptsdir/field_copy.php --type=rides --targetfield=ride_start_location_value ride_start_location   

  # content-type rides multis
  drush $targetalias scr $scriptsdir/field_copy.php --kind=fid ride_image
  drush $targetalias scr $scriptsdir/field_copy.php --kind=uid ride_waitlist
  drush $targetalias scr $scriptsdir/field_copy.php --kind=fid ride_attachments 

  # TODO: more content-type copies here: events, regions, cuesheets, mb (forumn topic), obride
  # ALSO: comments (forum and ?)
  # LATER OR NEVER: archives? block pages? blog entry? date? incentive request? story? webform? volunteer? others?
}

# RUN MIGRATION "CLEANUP" SCRIPTS ON TARGET
function nycc_migrate_cleanup_target() {
  echo "Cleanup target..."
  echo "Convert passwords..."
  # convert users passwords for use with d7
  drush $targetalias scr $scriptsdir/users-convert-pass.php > $tmpdir/users-convert-pass.out

  echo "Convert files..."
  # convert files for d7 use
  drush $targetalias scr $scriptsdir/users-convert-pictures.php > $tmpdir/users-convert-pictures.out
  
  # TODO: Additional processing:
  # TODO: concat d6 ride_from and (stripped) ride_from_select into ride_start_location on d7
  # TODO: strip map link from ride_from_select?
  # load/save all nodes and users to trigger other modules hooks?
  # in particular, check that profile2 pid's work as expected. what is test for this?
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
  mysql -uroot -pXt2792b8cf -e"SELECT COUNT(*) as users FROM $targetdb.users; SELECT COUNT(*) as nodes FROM $targetdb.node; "
}

# TODO: output basic counts for comparison
function nycc_migrate_report_source() {
  echo "nycc_migrate_report_source"
  mysql -uroot -pXt2792b8cf -e"SELECT COUNT(*) as users FROM $sourcedb.users; SELECT COUNT(*) as nodes FROM $sourcedb.node; "
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
  nycc_migrate_init_migration
fi


if [ -z "$backups" ]
then
  echo "Skipped: Backups (-b)"
else
  nycc_migrate_backup
fi


if [ -z "$production_sync" ]
then
  echo "Skipped: Production sync (-p)"
else
  nycc_migrate_export_production
  nycc_migrate_sync_production_to_source
fi


if [ -z "$import_production_to_source" ]
then
  echo "Skipped: Import production to source (-d)"
else
  nycc_migrate_import_production_to_source
fi


if [ -z "$clear_target" ]
then
  echo "Skipped: Clear target (-c)"
else
  nycc_migrate_clear_target
fi


if [ -z "$copy_source_to_target" ]
then
  echo "Skipped: Copy source to target (-a)"
else
  nycc_migrate_copy_source_to_target
fi


if [ -z "$cleanup_target" ]
then
  echo "Skipped: Cleanup target (-w)"
else
  nycc_migrate_cleanup_target
fi


if [ -z "$cleanup_migration" ]
then
  echo "Skipped: Cleanup migration (-x)"
else
  nycc_migrate_cleanup_migration
fi


if [ -z "$report_source" ]
then
  echo "Skipped: Report source (-y)"
else
  nycc_migrate_report_source
fi


if [ -z "$report_target" ]
then
  echo "Skipped: Report target (-z)"
else
  nycc_migrate_report_target
fi


if [ -z "$test" ]
then
#  echo "Skipped: test (-t)"
  echo "";
else
  echo "Test run."
  echo "Exec: drush $targetalias scr $scriptsdir/test.php"
  drush $targetalias scr $scriptsdir/test.php
fi



echo "Migration tasks completed."
