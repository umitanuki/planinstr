planinstr
=========

Synopsis
--------

    db1=# load '$libdir/planinstr';
    LOAD
    db1=# explain select * from pg_class;
                              QUERY PLAN                           
    ---------------------------------------------------------------
     Seq Scan on pg_class  (cost=0.00..141.87 rows=3287 width=194)
     Plan time: 0.119 ms
    (2 rows)

Description
-----------

This module provides a plugin functionality to measure planner
running time and append the result to the EXPLAIN output.

Support
-------

Goto http://github.com/umitanuki/planinstr

Author
------

Hitoshi Harada

