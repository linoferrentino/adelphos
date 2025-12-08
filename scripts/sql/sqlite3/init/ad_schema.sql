-- Adelphos database, schema screation.
-- Creates an empty schema.

-- the configuration table.
create table adelphos_conf(
	key text,
	value text,
	primary key (key)
) without rowid;

-- increment this for every change
insert into adelphos_conf(key, value) values ("schema_version", "0.11");


create table currency (
	cur_id integer primary key,

	name   text,

	-- this is the usually one character used to visualize it
	symbol text,
	
	-- the value which is stored in FOREX
	forex_string,

	-- human value, the amount of currency which is used to make one Tao
	hv real

);

-- just some currencies
begin;
insert into currency(cur_id, name, symbol, forex_string, hv) values(0, 'Euro', '€', 'EUR', 1);
insert into currency(cur_id, name, symbol, forex_string, hv) values(1, 'Dollar', '$', 'USD', 1);
insert into currency(cur_id, name, symbol, forex_string, hv) values(2, 'English Pound', '£"', 'GBP', 0.76);
insert into currency(cur_id, name, symbol, forex_string, hv) values(3, 'Swiss Frank', '₣', 'CHF', 0.81);
insert into currency(cur_id, name, symbol, forex_string, hv) values(4, 'Yen', '¥', 'JPY', 100);
commit;
--- other will follow...


-- a group can have a level
-- the group name must be unique within a level
create table ln_group (

	group_id integer primary key,

	-- this is the parent group for myself.
	parent_group_id integer

	-- lev integer,

	-- name text

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


	-- their password encrypted. 
	ad_pass text,

	-- 0 adult, 1 minor
	is_minor integer,

	-- an adelphos is part of a group of level zero, their family
	l0_id integer,

	-- every adelphos has a counter for their cheques
	-- cheque_next_id integer,
       
	foreign key (l0_id) references l0_group (group_id)
		on delete restrict,

	-- the email is unique in the system.
	unique (email) on conflict abort
);

create table position (

	position_id integer primary key,

	position_text text,

	-- a position belongs to a group
	group_owner_id integer,

	service_fee integer

);


create table trust_travel (

	pos_1 integer not null,
	pos_2 integer not null,

	-- the person who takes the person
	ad_carrier_id integer not null,

	-- how many people am I willing to take with me?
	n_people integer,

	-- schedule? Automatic? Manual?
	primary key (pos_1, pos_2, ad_carrier_id)


) without rowid;


-- somebody is willing to carry an object from one place to another
create table trust_carrier (

	-- from a position to another.
	pos_1 integer not null,
	pos_2 integer not null,

	-- the person who takes the object.
	ad_carrier_id integer not null,

	maximum_weigth real,
	maximum_dimension real,

	cost integer,

	-- schedule? Automatic? Manual?

	primary key (pos_1, pos_2, ad_carrier_id)

) without rowid;


create table announce (

	announce_id integer primary key,

	-- the person who holds the item
	ad_id integer,

	-- where do I want the item to be collected.
	pos_id integer,

	text_announce text,

	cost_item integer

);


-- a trust_channel is between two adelphoi or between two groups
-- if the trust line is between two users (l0) the l1 groups in
-- which they belong gets connected.

create table trust_line (


	-- If I have the role of admin my trust line is
	-- between two groups, but this won't survive if
	-- I change the admin.
	ad_1 integer not null,
	ad_2 integer not null,

	-- the trust line is fractal, it has an integer.
	lev integer,

	-- the trust line can be between two groups.

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


