create table if not exists url_mapping (
	id bigint generated always as identity (start with 100000 increment by 1) primary key,
	short_url varchar(10) unique not null,
	long_url text unique not null,
	created_at timestamp with time zone default current_timestamp,
	expires_at timestamp with time zone default (current_timestamp + '1 year')
);

create index idx_expires_at on url_mapping(expires_at);
create index idx_created_at on url_mapping(created_at);
create index idx_long_url on url_mapping(long_url);