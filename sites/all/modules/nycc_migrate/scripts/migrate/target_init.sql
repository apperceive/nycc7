TRUNCATE users_roles;
TRUNCATE role;
DELETE FROM users WHERE uid > 1;
TRUNCATE node;
TRUNCATE node_revision;
TRUNCATE file_managed;
TRUNCATE comment;
TRUNCATE field_data_comment_body;

DELETE FROM variable WHERE name LIKE 'backup_migrate_%';
DELETE FROM variable WHERE name LIKE 'nodesquirrel_%';

/* Remove autoloads using targetdir that is wiped */
/* TODO: pass part of targetdir here? */
/*DELETE FROM registry WHERE FILENAME like "%backup_migrate%";*/
DELETE FROM registry WHERE FILENAME LIKE "sites/default/files/%";
DELETE FROM cache_bootstrap WHERE DATA LIKE "%backup_migrate%";

/* all field data and revision tables are now truncated by default in field_copy.php */


