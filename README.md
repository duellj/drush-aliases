Drush Aliases
=============

Helpful Drush aliases that make my life more sane.

Current commads:

`drush grant-devel`
-------------------

Enables the `devel` module and grants all users access to devel information.

`drush rollback-update [module] [schema_version]`
-------------------------------------------------

Rolls back the schema version for the given module.  Can either be an absolute
value (e.g. `7001`) or a relative value (e.g. `-2`).  Defaults to `-1`.
