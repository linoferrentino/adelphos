-- Adelphos database, schema screation.
-- Creates an empty schema.

-- this is the data database
-- there is then the session/gui database which holds
-- temporary data, but it is not here.
.open ad_db.sqlite3


-- the configuration table.
create table adelphos_conf(
	key text,
	value text,
	primary key (key)
) without rowid;

-- increment this for every change
insert into adelphos_conf(key, value) values ("schema_version", "2");
-- defaults for the channels
insert into adelphos_conf(key, value) values ("default_l0_trust", "25");
-- how much the internal trust increments for every cycle.
insert into adelphos_conf(key, value) values ("default_l0_increment", "0.1");



create table currency (
	cur_id integer primary key,

	name   text,

	-- this is the symbol as it is written in the Forex exchange
	symbol text

);

-- just some currencies
insert into currency(cur_id, name, symbol) values(0, 'Euro', 'EUR');
insert into currency(cur_id, name, symbol) values(1, 'Dollar', 'USD');
insert into currency(cur_id, name, symbol) values(2, 'English Pound', 'GBP');
insert into currency(cur_id, name, symbol) values(3, 'Swiss Frank', 'CHF');
insert into currency(cur_id, name, symbol) values(4, 'Yen', 'JPY');
--- other may follow...


-- a group can have a level
-- the group name must be unique within a level
create table ln_group (

	group_id integer primary key,

	-- this is the parent group for myself.
	parent_group_id integer,

	lev integer,

	name text

	-- there is not a joining password, the ln_group is
	-- automatically instantiated.

);

-- the l0 and l1 groups are particular, because they have the credit limits
-- l0 group has also the minor password, because it is intended that l0 groups are families
create table l0_group (

	group_id integer primary key,

	-- this is the parent group for myself.
	-- an l0 group is part of a l1 group, always.
	parent_group_id integer,

	name text,

	-- administrator_id integer,

	-- the joining password for adults.
	-- join_pass_adult text,

	-- the joining passowrd for minors
	-- join_pass_minor text,

	-- the credit limit for this group, for adults
	credit_limit_adults real,

	-- the credit limit for this group, for minors
	credit_limit_minors real

);

-- l1 group can have a certain number of administrators.
-- ln groups do not have an administrator, they are reificated
-- when two adelphoi create a trust channel and they do not
-- have "member" in the common sense.
create table l1_group (

	group_id integer primary key,

	-- this is the parent group for myself.
	parent_group_id integer,

	name text,

	cur_id integer,

	foreign key (cur_id) references currency (cur_id)

	-- a l1_group has a currency, and all adelphoi share
	-- the same currency.
	-- inter currency trading can be done between two adelphoi of different ln (n >= 1) groups

	-- the credit limit for this group,
	-- no member of this group can have a credit/debit above
	-- this limit.
	-- credit_limit real

);


-- the adelphos table stores the data of an adelphos (brother)
create table adelphos (

	ad_id integer primary key,

	name text,

	-- just to have some identification.
	email text,

	phone text,

	-- her password encrypted. 
	ad_pass text,

	-- 0 adult, 1 minor, if adult he/she is also administrator
	is_minor integer,

	-- an adelphos is part of a group of level zero, his/her family
	l0_id integer,

	-- every adelphos has a counter for his/her cheques
	cheque_next_id integer,

	-- this is the credit limit inside the l1 group is written in the
	-- family 
	-- credit_limit real,

	-- it is not really an interest, because it has not an expiry date.
	-- it is the commission I charge for a credit to pass through me
	--commission real,

	-- the system tries to balance these two amounts, if I pay too much
	-- I lower the rate around me
	-- if I pay too little I raise the rate around me TBD
	-- total_commissions_paid integer,
	-- total_commissions_gained integer,
       
	foreign key (l0_id) references l0_group (group_id)

	-- every adelphos can have a certain number of cheques
);

-- only the l1 members are stored in a table, as only l1
-- groups are totally connected.
create table l1_members (

	l1_group_id integer,
	l0_group_id integer,
	is_administrator integer,

	primary key (l0_group_id, l1_group_id),
	foreign key (l0_group_id) references l0_group (group_id),
	foreign key (l1_group_id) references l1_group (group_id)

) without rowid;



-- a trust_channel is between two users of different l1 groups 
-- the groups can then be part of the same l2 group, or the same l3 group or...
create table trust_channel (

	ad_1 integer not null,
	ad_2 integer not null,

	-- the channel holds two groups together of the same level,
	-- at least of level 2
	-- but the channel is ALWAYS between two friends which have
	-- a super group in common.
	g1_id integer,
	g2_id integer,

	-- the trust of the channel, measured in db, the two parties
	-- must agree on a certain trust level (the enforcing trust is
	-- the least of the two)
	trust real,

	-- the two adelphoi must agree on a certain commission which other
	-- users will pay to use this credit line
	commission real,

	-- there is no rate, any more, the rate is the capacity of the channel
	primary key (ad_1, ad_2),

	foreign key(ad_1) references adelphos(ad_id),

	foreign key(ad_2) references adelphos(ad_id)

) without rowid;


-- now I can have a bunch of cheques, which are credits from l1 adelphoi
-- the credits are always in the currency of the sender!
-- the cheques of a person circulate only in l1 of that person
-- so the cheque does not propagate in a large network.
create table cheque (

	-- a monotonically increasing for the cheque in the source object
	cheque_id integer,

	-- the source of the cheque, this NEVER changes, also in case
	-- of splitting.
	ad_from integer,

	-- a cheque can be splitted in various amounts, at the end they will be collected.
	split_n integer,

	-- the destination of the cheque, the destination MUST be in the same l1 group 
	ad_to integer,

	amount integer,

	-- this is the amount of real money this cheque has 
	amount_real integer,

	-- the issue date, in case of splitting this is the split date
	issued_on text,

	-- the cheque must be in paid state, otherwise it is not valid
	state integer,

	-- a comment which has been appended, when we split a comment stays
	-- the same and another changed.
	purpose text,

	primary key (cheque_id, ad_from, split_n),

	foreign key (ad_from) references adelphos(ad_id) on delete cascade,

	foreign key (ad_to) references adelphos(ad_id) on delete cascade

) without rowid;


