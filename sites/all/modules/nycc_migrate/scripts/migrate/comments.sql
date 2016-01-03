REPLACE INTO `comment`
(`cid`,
`pid`,
`nid`,
`uid`,
`subject`,
`hostname`,
`changed`,
`status`,
`thread`,
`name`,
`mail`,
`homepage`,
`language`,
`created`, 
`uuid`)
SELECT `comments`.`cid`,
    `comments`.`pid`,
    `comments`.`nid`,
    `comments`.`uid`,
    `comments`.`subject`,
    `comments`.`comment`,
    `comments`.`hostname`,
    `comments`.`timestamp`,
    `comments`.`status`,
    `comments`.`thread`,
    `comments`.`name`,
    `comments`.`mail`,
    `comments`.`homepage`,
    'und',
    `comments`.`timestamp`,
    UUID()
FROM `d6test`.`comments`;
