-- Adelphos database, schema screation.
-- Creates an empty schema.

-- this is the data database
-- there is then the session/gui database which holds
-- temporary data, but it is not here.
--.open ad_db.sqlite3


-- the configuration table.
create table adelphos_conf(
	key text,
	value text,
	primary key (key)
) without rowid;

-- increment this for every change
insert into adelphos_conf(key, value) values ("schema_version", "3");




create table currency (
	cur_id integer primary key,

	name   text,

	-- this is the symbol as it is written in the Forex exchange
	symbol text,

	-- human value, the amount of currency which is used to make one Tao
	hv real

);

-- just some currencies
insert into currency(cur_id, name, symbol, hv) values(0, 'Euro', 'EUR', 1);
insert into currency(cur_id, name, symbol, hv) values(1, 'Dollar', 'USD', 1);
insert into currency(cur_id, name, symbol, hv) values(2, 'English Pound', 'GBP', 1);
insert into currency(cur_id, name, symbol, hv) values(3, 'Swiss Frank', 'CHF', 1);
insert into currency(cur_id, name, symbol, hv) values(4, 'Yen', 'JPY', 100);
--- other will follow...


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
	-- an l0group without a parent is not much useful
	parent_group_id integer,

	name text,

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

	email text,

	phone text,

	-- every adelphos has its own public key.
	public_key blob,

	-- the private key is NOT stored, but only visualized one time

	-- her password encrypted. 
	ad_pass text,

	-- 0 adult, 1 minor
	is_minor integer,

	-- an adelphos is part of a group of level zero, his/her family
	l0_id integer,

	-- every adelphos has a counter for his/her cheques
	cheque_next_id integer,

       
	foreign key (l0_id) references l0_group (group_id),

	-- the email is unique in the system.
	unique (email) on conflict abort
);

-- only the l1 members are stored in a table, as only l1
-- groups are totally connected.

-- No! All groups are totally connected, we do not have
-- the members here.
/*
create table l1_members (

	l1_group_id integer      not null,
	l0_group_id integer      not null,
	is_administrator integer not null,

	primary key (l0_group_id, l1_group_id),
	foreign key (l0_group_id) references l0_group (group_id),
	foreign key (l1_group_id) references l1_group (group_id)

) without rowid;
*/



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


