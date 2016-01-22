/* needed after reference to target sync */

DELETE FROM variable WHERE name LIKE 'backup_migrate_%';
DELETE FROM variable WHERE name LIKE 'nodesquirrel_%';

/* Remove autoloads using targetdir that is wiped */
DELETE FROM registry WHERE FILENAME LIKE "sites/default/files/%";
DELETE FROM cache_bootstrap WHERE DATA LIKE "%backup_migrate%";
