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
  /* 'reg_code' field */
  $data['invite']['reg_code'] = array(
    'title' => t('Registration code'),
    'help' => t('Issued registration code and acts as primary identifier for a invite.'),
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
  /* 'email' field */
  $data['invite']['email'] = array(
    'title' => t('Invite email adress'),
    'help' => t('E-mail the invite has been addressed to.'),
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
