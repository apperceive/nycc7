-- assume current db
-- use vars for db, user, folders, etc

-- insert into users () (select * from d6test.users)

select max(uid) from d6test.users;
select max(uid) from markus.users;


SELECT *, UNIX_TIMESTAMP()-access, ( UNIX_TIMESTAMP()-access)/(365*24*60*60)
FROM `d6test`.`users` LIMIT 1000;


SELECT uid, name, mail, access, created, login, ( UNIX_TIMESTAMP()-access)/(365*24*60*60)
FROM `d6test`.`users` order by 7;


SELECT COUNT(*)
FROM `d6test`.`users`;

SELECT COUNT(*)
FROM `d6test`.`users`
WHERE (status > 0) AND (uid > 0) AND ((UNIX_TIMESTAMP()-access) > 3*365*24*60*60)
