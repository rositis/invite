<?php

/**
 * @file
 * Rules integration for the invite module.
 */

/**
 * Implements hook_rules_event_info().
 */
function invite_rules_event_info() {
  return array(
    'invite_withdraw' => array(
      'label' => t('An invitation has been withdrawn'),
      'group' => t('Invite'),
      'variables' => array(
        'invite' => array('type' => 'invite', 'label' => t('Invitation')),
      ),
    ),
    'invite_accept' => array(
      'label' => t('An invitation has been accepted'),
      'group' => t('Invite'),
      'variables' => array(
        'invite' => array('type' => 'invite', 'label' => t('Invitation')),
        'account' => array('type' => 'user', 'label' => t('User that accepted the request')),
      ),
    ),
  );
}
