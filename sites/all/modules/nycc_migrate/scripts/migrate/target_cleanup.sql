/* Post migration cleanup tasks */

/* make apperceptions user an administrator */
INSERT INTO users_roles (uid, rid) VALUES (3692, 6);