create table College(
	cName varchar(50) not null,
    state varchar(50) not null,
    enrollment integer not null,
    primary key (cName)
);

create table Student(
	sID integer not null,
    sName varchar(50) not null,
    GPA float not null,
    sizeHS integer not null,
    primary key (sID)
);

create table Apply(
	sID integer not null,
    cName varchar(50) not null,
    major varchar(50) not null,
    decision char(5) not null,
    primary key (sID, cName, major)
);