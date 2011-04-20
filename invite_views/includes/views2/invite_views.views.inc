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
  /* fields */
  /* 'iid' field */
  $data['invite']['iid'] = array(
    'title' => t('Iid'),
    'help' => t('The invite ID.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => FILTER_FORMAT_DEFAULT,
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}
