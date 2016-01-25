/* Post migration cleanup tasks */

/* make apperceptions user an administrator */
INSERT INTO users_roles (uid, rid) VALUES (3692, 6);

/* convert nid's to uid's for leaders */
/* WARNING: NOT IDEMPOTENT! and do not run after REPLACE!!!! */
/* only apply to column when these are nids!!! */

UPDATE `field_data_field_ride_current_leaders` l INNER JOIN node nr ON nr.nid=l.entity_id AND nr.vid=l.revision_id INNER JOIN node np ON l.field_ride_current_leaders_uid = np.nid AND np.type= 'profile' SET field_ride_current_leaders_uid = np.uid;

UPDATE `field_revision_field_ride_current_leaders` l INNER JOIN node nr ON nr.nid=l.entity_id AND nr.vid=l.revision_id INNER JOIN node np ON l.field_ride_current_leaders_uid = np.nid AND np.type= 'profile' SET field_ride_current_leaders_uid = np.uid;

DELETE FROM node WHERE type IN ('gpcaptspost', 'test_users', 'block_page', 'poll2', 'webforms');
/* TODO: clear out all field data and rev tables that have invalid entity_id's */

DELETE FROM field_config_instance WHERE entity_type = 'node' AND bundle = 'profile';

DELETE FROM node WHERE type = 'profile' AND NOT uid IN (SELECT uid FROM users WHERE uid > 0);


/* TODO: de-dup profile nodes */

