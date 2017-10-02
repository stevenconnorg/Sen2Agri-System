begin transaction;

do $migration$
declare _statement text;
begin
    raise notice 'running migrations';

    if exists (select * from information_schema.tables where table_schema = 'public' and table_name = 'meta') then
        if exists (select * from meta where version = '1.6.2') then
            raise notice 'upgrading from 1.6.2 to 1.7';

            raise notice 'applying 06122c0b0a8294b27f6b18fe899609b0118ff2b2';
            _statement := 'update config_metadata set type = ''string'' where key = ''processor.l4a.reference_data_dir'';';
            raise notice '%', _statement;
            execute _statement;

            raise notice 'applying 00d4d2597c599f0d30cbc7c5b2a7662c45126db1';
            if not exists (select * from downloader_status where status_description = 'processing_failed') then
                raise notice 'INSERT INTO downloader_status VALUES (6, ''processing_failed'');';
                INSERT INTO downloader_status VALUES (6, 'processing_failed');
            end if;

            _statement := 'update meta set version = ''1.7'';';
            raise notice '%', _statement;
            execute _statement;
        end if;
    end if;

    raise notice 'complete';
end;
$migration$;

commit;
