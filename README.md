planinstr
=========

planinstr provides a plugin routines to measure planner
running time. Once the module is loaded into the backend,
additional row that tells "Plan Time" at the end of output
rows of EXPLAIN.

To build it, just do this:

    make
    make install

If you encounter an error such as:

    make: pg_config: Command not found

Be sure that you have `pg_config` installed and in your path. If you used a
package management system such as RPM to install PostgreSQL, be sure that the
`-devel` package is also installed. If necessary tell the build process where
to find it:

    env PG_CONFIG=/path/to/pg_config make && make installcheck && make install

Once `make install` is done, connect your database with psql or other client
and type

    LOAD '$libdir/planinstr'

then throw EXPLAIN query. If you prefere the loading operation be automated,
add an entry of shared\_preload\_library in postgresql.conf.

Dependencies
------------
The `planinstr` data type has no dependencies other than PostgreSQL.

