/* clear out migration target data plus any data that causes problems for migration or new site (eg, registry entries) */

/* TODO: pass part of targetdir here? */ 


/* NOTE: all field data and revision tables are now truncated by default in field_copy.php */


TRUNCATE users_roles;
TRUNCATE role;
DELETE FROM users WHERE uid > 1;
TRUNCATE node;
TRUNCATE node_revision;
TRUNCATE file_managed;
TRUNCATE comment;
TRUNCATE field_data_comment_body;
TRUNCATE profile;
TRUNCATE history;
TRUNCATE node_comment_statistics;

DELETE FROM variable WHERE name LIKE 'backup_migrate_%';
DELETE FROM variable WHERE name LIKE 'nodesquirrel_%';

/* Remove autoloads using targetdir that is wiped */
DELETE FROM registry WHERE FILENAME LIKE "sites/default/files/%";
DELETE FROM cache_bootstrap WHERE DATA LIKE "%backup_migrate%";
