REPLACE INTO `file_managed`
(`fid`,
`uid`,
`filename`,
`uri`,
`filemime`,
`filesize`,
`status`,
`timestamp`,
`uuid`)
SELECT `files`.`fid`,
    `files`.`uid`,
    `files`.`filename`,
    REPLACE(`files`.`filepath`,'sites/default/files','public://'),
    `files`.`filemime`,
    `files`.`filesize`,
    `files`.`status`,
    `files`.`timestamp`,
    UUID()
FROM `d6test`.`files` WHERE filepath LIKE 'sites/default/files%'
