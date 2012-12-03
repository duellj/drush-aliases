<?php
/**
 * @file
 * Provide Drush integration for complex aliases.
 */

/**
 * Implements hook_drush_command().
 */
function drush_aliases_drush_command() {
  $items = array();

  $items['grant-devel'] = array(
    'description' => 'Grants all users devel access',
    'callback' => 'drush_drush_aliases_grant_devel',
  );

  $items['rollback-update'] = array(
    'description' => 'Rollsback schema version for given module.',
    'callback' => 'drush_drush_aliases_rollback_update',
    'arguments' => array(
      'module' => 'The module to change schema version for',
      'version' => 'The version to rollback to',
    ),
    'required-arguments' => 1,
    'examples' => array(
      'drush rollback-update custom_module -2' => 'Updates the schema version for custom_module to 2 behind the current version.  Allows the last two update functions to be rerun.',
    ),
  );

  return $items;
}

/**
 * Grant devel access to all users.
 *
 * Before calling this we need to be bootstrapped to DRUPAL_BOOTSTRAP_DATABASE.
 */
function drush_drush_aliases_grant_devel() {
  if (!module_exists('devel')) {
    module_enable(array('devel'));
  }

  user_role_grant_permissions(DRUPAL_ANONYMOUS_RID, array('access devel information'));
  user_role_grant_permissions(DRUPAL_AUTHENTICATED_RID, array('access devel information'));
}

/**
 * Rollback schema version for given module.
 */
function drush_drush_aliases_rollback_update($module, $new_version = -1) {
  if (!module_exists($module)) {
    drush_log('Module ' . $module . ' not found', 'error');
  }

  $version = db_query('SELECT schema_version FROM {system} WHERE name = :module', array(':module' => $module))->fetchField();

  if ($new_version < 0) {
    $version += $new_version;
  }
  else {
    $version = $new_version;
  }

  db_update('system')
    ->fields(array(
      'schema_version' => $version,
    ))
    ->condition('name', $module)
    ->execute();

  drush_log('Module ' . $module . ' updated to schema version ' . $version, 'ok');
}
