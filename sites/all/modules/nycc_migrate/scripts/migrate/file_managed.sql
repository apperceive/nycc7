REPLACE INTO $targetdb`file_managed`
(`fid`,
`uid`,
`filename`,
`uri`,
`filemime`,
`filesize`,
`status`,
`timestamp`)
SELECT `files`.`fid`,
    `files`.`uid`,
    `files`.`filename`,
    REPLACE(`files`.`filepath`,'sites/default/files','public://'),
    `files`.`filemime`,
    `files`.`filesize`,
    `files`.`status`,
    `files`.`timestamp`
FROM $sourcedb`files` WHERE filepath LIKE 'sites/default/files%'
