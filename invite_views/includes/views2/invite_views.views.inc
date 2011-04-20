<?php

/**
 * @file
 */

/**
 * Implementation of hook_views_data().
 */
function invite_views_views_data() {
  /* 'invite' table */
  $data['invite']['table']['group'] = 'Invite';
  $data['invite']['table']['base'] = array(
    'field' => 'iid',
    'title' => t('Invite'),
    'help' => t('The base table for invites.'),
  );
  return $data;
}
