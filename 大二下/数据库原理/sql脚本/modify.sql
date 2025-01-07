insert into College values('Carnegie Mellon', 'PA', 11500);


insert into Apply(sID, cName, major, decision)
select S.sID, 'Carnegie Mellon', 'CS', null
from Student S
where not exists(
	select *
    from Apply A
    where A.sID = S.sID
);


insert into Apply(sID, cName, major, decision)
select sID, 'Carnegie Mellon', 'EE', 'Y'
from Apply
where major = 'EE' and decision = 'N';


delete from Student
where sID in(
	select sID from Apply
    group by sID having count(distinct major) > 2
);


delete from College
where not exists(
	select * from Apply
    where Apply.cName = College.cName
    and Apply.major = 'CS'
);


update Apply
set decision = 'Y', major = 'economics'
where cName = 'Carnegie Mellon'
and sID in(select sID 
from Student where GPA < 3.6
);






SET @maxGPA := (
  SELECT MAX(s.GPA)
  FROM Apply a
  JOIN Student s ON a.sID = s.sID
  WHERE a.major = 'EE'
);

UPDATE Apply a
JOIN Student s ON a.sID = s.sID
SET a.major = 'CSE'
WHERE a.major = 'EE' AND s.GPA = @maxGPA;
