create table if not exists url_mapping (
	id bigserial primary key,
	short_url varchar(10) unique not null,
	long_url text not null,
	created_at timestamp with time zone default current_timestamp,
	expires_at timestamp with time zone default (current_timestamp + '1 year')
);

create index idx_expires_at on url_mapping(expires_at);