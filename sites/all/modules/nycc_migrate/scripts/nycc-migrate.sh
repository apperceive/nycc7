#~/bin/bash

site="markus"
user="markus"
scriptsdir="/var/www/html/$user/sites/all/modules/nycc_migrate/scripts/migrate"
targetalias="@$siteTest"
targetdb="$site"
targetdir=/var/www/html/$site/sites/default/files
sourcedb=d6test
sourcedir="$user@nycc.org:/var/www/html/nycc/sites/default/files/"
tmpdir=~/backups

# partial or full export of procduction database
# ssh markus@nycc.org "mysqldump -q -uroot -pXt2792b8cf nycc node content_type_rides  > $tmpdir/production.sql"
ssh markus@nycc.org "mysqldump -$sourcedb -uroot -pXt2792b8cf nycc > $tmpdir/production.sql"

# do not drop if using partial table updates
mysql -uroot --pXt2792b8cf -e"DROP DATABASE $sourcedb; CREATE DATABASE $sourcedb;"

mysql -uroot --pXt2792b8cf  < $tmpdir/production.sql


# update files
sudo rsync -avzu -e --exclude="*.gz" --exclude="js" --exclude="css" --exclude="imagecache" $sourcedir $targetdir

# run migration scripts
mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/roles.sql
mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/users.sql

# fix passwords
drush $targetalias scr $scriptsdir/users-convert-pass.php
drush $targetalias scr $scriptsdir/users-convert-pictures.php

mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/users-roles.sql
mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/node.sql
mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/node_revision.sql
mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/file_managed.sql

# content-type-page
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_field_carousel_order.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_field_date.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_field_image_cache.sql

# todo: add vars for folders 
drush scr migrate/field_copy.php field_carousel_order
drush scr migrate/field_copy.php field_date
drush scr migrate/field_copy.php --kind=fid field_image_cache 


# NOTE: field-copy.php not currently working with aliases so need to cd and omit

# content-type-rides

drush $targetalias scr $scriptsdir/field_copy.php --type=rides ride_description ride_type ride_select_level ride_speed ride_type ride_distance_in_miles ride_signups ride_start_location ride_spots ride_cue_sheet ride_current_leaders ride_status ride_open_signup_days ride_token ride_from ride_from_select ride_timestamp

drush $targetalias scr $scriptsdir/field_copy.php ride_image ride_waitlist ride_attachments

drush scr migrate/field_copy.php --kind=fid ride_image
drush scr migrate/field_copy.php --kind=uid ride_waitlist
drush scr migrate/field_copy.php --kind=fid ride_attachments 


# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_description.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_type.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_select_level.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_speed.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_type.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_distance_in_miles.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_signups.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_start_location.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_spots.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_image.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_cue_sheet.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_attachments.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_current_leaders.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_waitlist.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_status.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_open_signup_days.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_token.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_from.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_from_select.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_rwgps_link.sql
# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_upload.sql

# mysql -uroot --pXt2792b8cf $targetdb < $scriptsdir/field_data_ride_timestamp.sql

# is there still a need for timestamp conversion/cleanup?  I think so.


# Additional processing:
#
# use drush/php scripts to populate target tables and fields
# riders from nid to uid : build/reuse a mapping table? 
# also, profile2 pid's : uid<-->nid(profile)<-->pid (profile2)
# password conversion - see script
# users.picture - see script
# rebuild imagecache - we need to first re-define or export these



