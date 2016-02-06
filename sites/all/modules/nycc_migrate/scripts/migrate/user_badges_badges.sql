REPLACE INTO $targetdb`user_badges_badges`
(`bid`,
`name`,
`image`,
`weight`,
`href`,
`tid`,
`unhideable`,
`fixedweight`,
`doesnotcounttolimit`)
SELECT `user_badges_badges`.`bid`,
    `user_badges_badges`.`name`,
    `user_badges_badges`.`image`,
    `user_badges_badges`.`weight`,
    `user_badges_badges`.`href`,
    `user_badges_badges`.`unhideable`,
    `user_badges_badges`.`fixedweight`,
    `user_badges_badges`.`doesnotcounttolimit`,
    `user_badges_badges`.`tid`
FROM $sourcedb`user_badges_badges`;
