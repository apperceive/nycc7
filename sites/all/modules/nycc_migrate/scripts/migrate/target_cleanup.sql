/* Post migration cleanup tasks */

/* make apperceptions user an administrator */
INSERT INTO users_roles (uid, rid) VALUES (3692, 6);

/* convert nid's to uid's for leaders */
UPDATE `field_revision_field_ride_current_leaders` l INNER JOIN node n ON n.nid=l.field_ride_current_leaders_uid SET field_ride_current_leaders_uid =n.uid AND n.type= 'profile';
