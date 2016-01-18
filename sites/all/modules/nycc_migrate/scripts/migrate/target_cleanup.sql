/* Post migration cleanup tasks */

/* make apperceptions user an administrator */
INSERT INTO users_roles (uid, rid) VALUES (3692, 6);

/* convert nid's to uid's for leaders */
/* WARNING: NOT IDEMPOTENT! and do not run after REPLACE!!!! */
/* only apply to column when these are nids!!! */
UPDATE `field_data_field_ride_current_leaders` l INNER JOIN node n ON n.nid=l.field_ride_current_leaders_uid SET field_ride_current_leaders_uid =n.uid AND n.type= 'profile';
UPDATE `field_revision_field_ride_current_leaders` l INNER JOIN node n ON n.nid=l.field_ride_current_leaders_uid SET field_ride_current_leaders_uid =n.uid AND n.type= 'profile';


DELETE FROM node WHERE type IN ('gpcaptspost', 'test_users', 'block_page', 'poll2', 'webforms')
/* TODO: clear out all field data and rev tables that have invalid entity_id's */

DELETE FROM field_config_instance WHERE entity_type = 'node' and bundle = 'profile';
