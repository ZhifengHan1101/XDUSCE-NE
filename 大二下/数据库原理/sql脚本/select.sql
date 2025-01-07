select sID, sName from Student where GPA > 3.6;


select distinct sName, major from Student join Apply 
where Student.sID = Apply.sID;


select distinct sName, GPA, decision from Student join Apply
where Student.sID = Apply.sID
and sizeHS < 1000 
and cName = 'Stanford' 
and major = 'CS';


select distinct cName from College 
where enrollment >= 20000
and cName in (select cName from Apply where major = 'CS');


select distinct Student.sID, Student.sName, Student.GPA, Apply.cName, Student.sizeHS 
from Student 
join Apply on Student.sID = Apply.sID
group by Apply.cName, Student.sID, Student.sName, Student.GPA, Student.sizeHS
order by Apply.cName, Student.GPA desc, Student.sizeHS;

select sID, major from Apply where major like '%bio%';


select distinct s1.sID, s1.sName, s1.GPA, s2.sID, s2.sName, s2.GPA
from Student s1 join Student s2
where s1.GPA = s2.GPA and s1.sID < s2.sID;


select distinct sID from Apply as A1
where major = 'CS' and exists(
	select * from Apply as A2
    where A1.sID = A2.sID and A2.major = 'EE'
);


select distinct sID, sName from Student
where exists(
	select * from Apply
    where Student.sID = Apply.sID and major = 'CS'
)and not exists(
	select * from Apply
    where Student.sID = Apply.sID and major = 'EE'
);


select distinct sID, sName from Student
where exists(
	select * from Apply
    where Student.sID = Apply.sID and major = 'CS'
);


select distinct cName, state from College
where state in(
	select state from College
    group by state having count(*) > 1
);


select cName from College
where enrollment = (
	select max(enrollment) from College
);


select sName, GPA from Student
order by GPA desc
limit 1;


select distinct sID, sName, sizeHS from Student
where sizeHS <> (
	select min(sizeHS) from Student
);


select C.cName, C.state, max(S.GPA) as max_GPA
from College C
join Apply A on C.cName = A.cName
join Student S on A.sID = S.sID
group by C.cName, C.state;


select distinct S.sName, A.major from Student S
join Apply A on S.sID = A.sID;


select distinct sName, GPA from Student
where sizeHS < 1000
and exists(
    select * from Apply
    where major = 'CS' and Student.sID = Apply.sID
);


select min(GPA) as min_GPA from Student S
join Apply A on S.sID = A.sID
where A.major = 'CS';


select avg(distinct GPA) as avg_GPA from Student S
join Apply A on S.sID = A.sID
where A.major = 'CS';


select count(distinct A.sID) as application_num
from Apply A join College C on A.cName = C.cName
where C.cName = 'Cornell';


select S.sID, S.sName, S.GPA, S.sizeHS from Student S
join(
	select GPA, count(distinct sID) as cnt1 
    from Student
    group by GPA
)T1 on S.GPA = T1.GPA
join(
	select sizeHS, count(distinct sID) as cnt2
    from Student
    group by sizeHS
)T2 on S.sizeHS = T2.sizeHS
where T1.cnt1 = T2.cnt2;


select avg(case when major = 'CS' then GPA else NULL end) as avg_cs,
		avg(case when major <> 'CS' then GPA else NULL end) as avg_no_cs,
        avg(case when major = 'CS' then GPA else NULL end)-
        avg(case when major <> 'CS' then GPA else NULL end) as diff
from Apply
join Student on Apply.sID = Student.sID;


SELECT cName, COUNT(sID) AS application_num
FROM Apply 
GROUP BY cName;
 

select A.cName, A.major, truncate((T.maxGPA - T.minGPA),1) as maxDiffGPA
from Apply A join(
	select cName, major, min(GPA) as minGPA, max(GPA) as maxGPA
    from Student S join Apply A on S.sID = A.sID
    group by cName, major
) T on A.cName = T.cName and A.major = T.major
group by A.cName, A.major
order by A.cName;


SELECT S.sID, COUNT(A.cName) AS application_num
FROM Student S LEFT JOIN Apply A ON S.sID = A.sID
GROUP BY S.sID;


select A.cName from Apply A
group by A.cName 
having count(distinct A.sID) < 5;


select A.major
from(
	select avg(GPA) as avg_GPA
    from Student
) as S,(
	select major, max(GPA) as max_GPA
    from Apply, Student
    where Apply.sID = Student.sID
    group by major
) as A
where A.max_GPA < S.avg_GPA;


