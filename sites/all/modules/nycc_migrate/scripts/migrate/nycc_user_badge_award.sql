REPLACE INTO $targetdb`nycc_user_badge_award`
(`uid`,
`bid`,
`awarded`)
SELECT `nycc_user_badge_award`.`uid`,
    `nycc_user_badge_award`.`bid`,
    `nycc_user_badge_award`.`awarded`
FROM $sourcedb`nycc_user_badge_award`;

