<?php

/**
 * @file
 * Administration functions for invite module.
 */

/**
 * Menu callback; display invite settings form.
 */
function invite_settings() {
  $permissions_url = url('admin/people/permissions', array('query' => drupal_get_destination(), 'fragment' => 'module-invite'));

  $roles = user_roles(FALSE, 'send invitations');
  if (count($roles) == 0) {
    drupal_set_message(t('Please enable the <em>send invitations</em> permission for at least one role on the <a href="@url">permissions page</a>.', array('@url' => $permissions_url)), 'warning');
  }
  $target_roles = user_roles(TRUE);

  // General settings.
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
  );
  $form['general']['invite_target_role_default'] = array(
    '#type' => 'select',
    '#title' => t('Default target role'),
    '#default_value' => variable_get('invite_target_role_default', DRUPAL_AUTHENTICATED_RID),
    '#options' => $target_roles,
    '#description' => t('Choose the default role that invited users will be added to when they register. For example, <em>authenticated user</em>.'),
    '#required' => TRUE,
  );
  $form['general']['invite_expiry'] = array(
    '#type' => 'select',
    '#title' => t('Invitation expiry'),
    '#default_value' => variable_get('invite_expiry', 30),
    '#options' => drupal_map_assoc(array(1, 3, 7, 14, 30, 60)),
    '#description' => t('Set the expiry period for user invitations, in days.'),
    '#multiple' => FALSE,
    '#required' => TRUE,
  );
  $form['general']['invite_registration_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to registration page'),
    '#default_value' => variable_get('invite_registration_path', 'user/register'),
    '#description' => t('Path to the registration page for invited users. Useful when using the <em>Assign from Path</em> option of <a href="@url">Auto Assign Roles</a> module.', array('@url' => 'http://drupal.org/project/autoassignrole')),
  );

  // Role settings.
  $form['role'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role settings'),
    '#description' => t('To control which roles may send invitations visit the <a href="@url">permissions page</a>.', array('@url' => $permissions_url)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
  );

  foreach ($roles as $rid => $role) {
    $form['role'][$rid] = array(
      '#type' => 'fieldset',
      '#title' => t('@role settings', array('@role' => drupal_ucfirst($role))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['role'][$rid]['invite_target_role_' . $rid] = array(
      '#type' => 'select',
      '#title' => t('Target role'),
      '#default_value' => variable_get('invite_target_role_' . $rid, DRUPAL_AUTHENTICATED_RID),
      '#options' => $target_roles,
      '#description' => t('You may choose to add invited users to another role (in addition to the default role set in the general section) when they have been invited by a member of %role.', array('%role' => $role)),
      '#required' => TRUE,
    );
    $form['role'][$rid]['invite_maxnum_' . $rid] = array(
      '#type' => 'select',
      '#title' => t('Invitation limit'),
      '#default_value' => variable_get('invite_maxnum_' . $rid, INVITE_UNLIMITED),
      '#options' => array(5 => 5, 10 => 10, 20 => 20, 50 => 50, 100 => 100, 500 => 500, 1000 => 1000, INVITE_UNLIMITED => t('unlimited')),
      '#description' => t('Allows to limit the total number of invitations members of %role can send.', array('%role' => $role)),
      '#multiple' => FALSE,
      '#required' => TRUE,
    );
  }

  // E-mail settings.
  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
  );
  $form['email']['invite_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get('invite_subject', t('[invite:inviter-raw] has sent you an invite!')),
    '#description' => t('Type the default subject of the invitation e-mail.') . ' ' . t('Use the syntax [token] if you want to insert a replacement pattern.'),
    '#required' => TRUE,
  );
  $form['email']['invite_subject_editable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Editable subject'),
    '#description' => t('Choose whether users should be able to customize the subject.'),
    '#default_value' => variable_get('invite_subject_editable', FALSE),
  );
  $form['email']['invite_default_mail_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail template'),
    '#default_value' => _invite_get_mail_template(),
    '#required' => TRUE,
    '#description' => t('Use the syntax [token] if you want to insert a replacement pattern.'),
  );
  $form['email']['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email']['token_help']['help'] = array(
    '#markup' => theme('token_tree', array('token_types' => array('user', 'profile', 'invite'))),
  );
  $form['email']['invite_use_users_email'] = array(
    '#type' => 'radios',
    '#title' => t('<em>From</em> e-mail address'),
    '#description' => t('Choose which e-mail address will be in the From: header for the invitation mails sent; <em>site</em> or <em>inviter</em>. <em>Site</em> will use the default e-mail address of the site, whereas <em>inviter</em> will use the e-mail address of the user who is sending the invitation. Alternatively, you can set this value manually by clicking on <em>advanced settings</em> below.'),
    '#options' => array(t('site'), t('inviter')),
    '#default_value' => variable_get('invite_use_users_email', 0),
  );
  $form['email']['invite_use_users_email_replyto'] = array(
    '#type' => 'radios',
    '#title' => t('<em>Reply-To</em> e-mail address'),
    '#description' => t('Choose which e-mail address will be in the Reply-To: header for the invitation mails sent; <em>site</em> or <em>inviter</em>. <em>Site</em> will use the default e-mail address of the site, whereas <em>inviter</em> will use the e-mail address of the user who is sending the invitation. Alternatively, you can set this value manually by clicking on <em>advanced settings</em> below.'),
    '#options' => array(t('site'), t('inviter')),
    '#default_value' => variable_get('invite_use_users_email_replyto', 0),
  );
  $form['email']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('<strong>Note:</strong> unless these fields are blank, they will override the radio button choices above.'),
  );
  $form['email']['advanced']['invite_manual_from'] = array(
    '#type' => 'textfield',
    '#title' => t('Manually override <em>From</em> e-mail address'),
    '#default_value' => variable_get('invite_manual_from', ''),
    '#description' => t('The e-mail address the invitation e-mail is sent from.'),
  );
  $form['email']['advanced']['invite_manual_reply_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Manually override <em>Reply-To</em> e-mail address'),
    '#default_value' => variable_get('invite_manual_reply_to', ''),
    '#description' => t('The e-mail address you want recipients to reply to.'),
  );

  // Invite page customization.
  $form['custom'] = array(
    '#type' => 'fieldset',
    '#title' => t('Invite page customization'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
  );
  $form['custom']['invite_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Invite page title'),
    '#default_value' => variable_get('invite_page_title', t('Invite a friend')),
    '#description' => t('The title of the page where users invite friends.'),
    '#required' => TRUE,
  );

  // Add vertical tabs display if available.
  $form['#pre_render'][] = 'vertical_tabs_form_pre_render';

  return system_settings_form($form);
}

/**
 * Return a list of all users that have invited someone.
 */
function invite_admin_overview() {
  $header = array(
    array('data' => t('Username'), 'field' => 'u1.name', 'sort' => 'asc'),
    array('data' => t('Total'), 'field' => 'invites'),
    t('Successful'),
    t('Pending'),
    t('Expired'),
    t('Remaining'),
    t('Operations'),
  );
  
  $query = array();

  $result = db_select('invite', 'i')
    ->fields('i', array('uid'))
    ->extend('TableSort')
    ->extend('PagerDefault');
  $result->innerJoin('users', 'u1', 'u1.uid = i.uid');
  $result->fields('u1', array('name'));
  $result->addExpression('COUNT(*)', 'invites');

  $count_query = db_select('invite', 'i');
  $count_query->innerJoin('users', 'u1', 'u1.uid = i.uid');
  $count_query->addExpression('COUNT(DISTINCT i.uid)', 'count');

  if (isset($_SESSION[INVITE_ADMIN_SESSION])) {
    $result->innerJoin('users', 'u2', 'u2.uid = i.invitee');
    $result->where("LOWER(u2.name) LIKE CONCAT('%', LOWER(:name), '%')", array(':name' => $_SESSION[INVITE_ADMIN_SESSION]));

    $count_query->innerJoin('users', 'u2', 'u2.uid = i.invitee');
    $count_query->where("LOWER(u2.name) LIKE CONCAT('%', LOWER(:name), '%')", array(':name' => $_SESSION[INVITE_ADMIN_SESSION]));
    
    $query = array('filter' => $_SESSION[INVITE_ADMIN_SESSION]);
    unset($_SESSION[INVITE_ADMIN_SESSION]);
  }

  $result->groupBy('i.uid');
  $result->groupBy('u1.name');
  $result->groupBy('u1.data');

  $result->setCountQuery($count_query);

  $result->orderByHeader($header);
  $result = $result->execute()->fetchAll();

  $output = render(drupal_get_form('invite_admin_filter_form'));

  $rows = array();
  foreach ($result as $row) {
    $account = user_load($row->uid);
    
    $cells = array();
    $cells[] = theme('username', array('account' => $row, 'name' => $row->name));
    $cells[] = $row->invites;
    $cells[] = invite_count($row->uid, 'accepted');
    $cells[] = invite_count($row->uid, 'pending');
    $cells[] = invite_count($row->uid, 'expired');
    $remaining = invite_get_remaining_invites($account);
    $cells[] = ($remaining == INVITE_UNLIMITED) ? '&infin;' : $remaining;
    $cells[] = l(t('details'), "admin/config/people/invite/details/$row->uid", array('query' => $query));
    $rows[] = $cells;
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'invite')));
  if (!$rows) {
    $output .= t('No inviters found.');
  }
  else {
    $output .= theme('pager');
  }

  return $output;
}

/**
 * Creates the user filter form for the admin overview.
 */
function invite_admin_filter_form() {
  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['filter']['filter'] = array(
    '#type' => 'textfield',
    '#title' => t('Find an invited user'),
    '#default_value' => isset($_SESSION[INVITE_ADMIN_SESSION]) ? $_SESSION[INVITE_ADMIN_SESSION] : '',
    '#autocomplete_path' => 'user/autocomplete',
    '#size' => 20,
  );
  $form['filter']['op'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  return $form;
}

function invite_admin_filter_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['filter'])) {
    $_SESSION[INVITE_ADMIN_SESSION] = $form_state['values']['filter'];
  }
  else {
    unset($_SESSION[INVITE_ADMIN_SESSION]);
  }
}

/**
 * Return a list of invites by a user.
 *
 * @param $account
 *   A user object.
 */
function invite_admin_details($account) {
  $header = array(
    array('data' => t('E-mail'), 'field' => 'i.email', 'sort' => 'asc'),
    array('data' => t('Username'), 'field' => 'u.name'),
    array('data' => t('Created'), 'field' => 'i.created'),
    array('data' => t('Expires'), 'field' => 'i.expiry'),
    array('data' => t('Status'), 'field' => 'i.canceled'),
  );

  $result = db_select('invite', 'i')
    ->fields('i')
    ->extend('TableSort')
    ->extend('PagerDefault');
  $result->leftJoin('users', 'u', 'u.uid = i.invitee AND u.uid <> 0');
  $result->condition('i.uid', $account->uid);

  $output = '';
  if (isset($_GET['filter']) && $_GET['filter'] != '') {
    $result->where("LOWER(u.name) LIKE CONCAT('%', LOWER(:name), '%')", array(':name' => $_GET['filter']));

    $output .= render(drupal_get_form('invite_admin_details_filter_form', $account->uid, $_GET['filter']));
  }

  $result = $result->orderByHeader($header)
    ->execute()
    ->fetchAll();

  if (isset($_GET['order']) && $_GET['order'] == t('Status')) {
    usort($result, 'invite_sort_by_status');
    if (isset($_GET['sort']) && $_GET['sort'] == 'desc') {
      $result = array_reverse($result);
    }
  }
  
  $rows = array();
  foreach ($result as $row) {
    $invitee = user_load($row->invitee);
    
    $cells = array();
    $cells[] = check_plain($row->email);
    $cells[] = $row->joined ? theme('username', array('account' => $invitee)) : '';
    $cells[] = format_date($row->created, 'short');
    $cells[] = format_date($row->expiry, 'short');
    $cells[] = $row->canceled ? t('Withdrawn') : ($row->joined ? t('Joined') : ($row->expiry < REQUEST_TIME ? t('Expired') : t('Pending')));
    $rows[] = $cells;
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'invite')));
  if (!$rows) {
    $output .= t('No invitees found.');
  }
  else {
    $output .= theme('pager');
  }

  return $output;
}

/**
 * Comparison function callback to sort invites by status.
 *
 * @param $a
 *   The first item to compare.
 * @param $b
 *   The second item to compare.
 */
function invite_sort_by_status($a, $b) {
  $status_a = $a->canceled ? t('Withdrawn') : ($a->joined ? t('Joined') : ($a->expiry < REQUEST_TIME ? t('Expired') : t('Pending')));
  $status_b = $b->canceled ? t('Withdrawn') : ($b->joined ? t('Joined') : ($b->expiry < REQUEST_TIME ? t('Expired') : t('Pending')));
    
  return $status_a < $status_b ? -1 : ($status_a > $status_b ? 1 : 0); 
}

function invite_admin_details_filter_form($form, &$form_state, $uid, $filter) {
  $form['#action'] = "/admin/config/people/invite/details/$uid";
  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['filter']['filter'] = array(
    '#markup' => check_plain($filter),
  );
  $form['filter']['op'] = array(
    '#type' => 'button',
    '#value' => t('Clear'),
    '#prefix' => ' ',
  );
  return $form;
}

/**
 * Menu callback; display an overview of sent invitations.
 *
 * @param $page
 *   Which invites to list: accepted, pending, or expired.
 */
function invite_user_overview($page, $account) {
  global $user;

  $rows = array();
  $profile_access = user_access('access user profiles');
  $allow_delete = user_access('withdraw accepted invitations');

  $query = db_select('invite', 'i')
    ->fields('i')
    ->extend('PagerDefault')
    ->condition('i.uid', $account->uid)
    ->condition('canceled', 0);

  switch ($page) {
    case 'accepted':
    // @TODO: I believe that if default is at the top it will always be
    // defaulted to and the other cases will never be hit.
    default:
      $query->leftJoin('users', 'u', 'u.uid = i.invitee AND u.uid <> 0');
      $query->fields('u', array('uid'));
      $query->condition('i.joined', 0, '<>');
      $query->orderBy('u.uid', 'DESC');
      //$sql = "SELECT i.*, u.uid AS account FROM {invite} i LEFT JOIN {users} u ON u.uid = i.invitee AND u.uid <> 0 WHERE i.uid = %d AND i.joined <> 0 AND canceled = 0 ORDER BY u.uid DESC";
      break;
    case 'pending':
      $query->condition('i.joined', 0);
      $query->condition('i.expiry', REQUEST_TIME, '>=');
      $query->orderBy('i.expiry', 'DESC');
      //$sql = "SELECT * FROM {invite} WHERE uid = %d AND joined = 0 AND canceled = 0 AND expiry >= %d ORDER BY expiry DESC";
      break;
    case 'expired':
      $query->condition('i.joined', 0);
      $query->condition('i.expiry', REQUEST_TIME, '<');
      $query->orderBy('i.expiry', 'DESC');
      //$sql = "SELECT * FROM {invite} WHERE uid = %d AND joined = 0 AND canceled = 0 AND expiry < %d ORDER BY expiry DESC";
      break;
  }

  //$result = pager_query($sql, 50, 0, NULL, $user->uid, REQUEST_TIME);
  $result = $query->execute()->fetchAll();

  foreach ($result as $invite) {
    $row = array();
    switch ($page) {
      case 'accepted':
      default:
        $account_exists = !is_null($invite->uid);

        if ($profile_access && $account_exists) {
          $row[] = l($invite->email, 'user/' . $invite->invitee, array('title' => t('View user profile.')));
        }
        else {
          $row[] = check_plain($invite->email);
        }
        $row[] = $account_exists ? t('Accepted') : t('Deleted');
        $row[] = $allow_delete ? l(t('withdraw'), "invite/withdraw/$invite->reg_code", array('query' => drupal_get_destination())) : '';
        break;

      case 'pending':
      case 'expired':
        $expired = ($invite->expiry < REQUEST_TIME);

        $row[] = check_plain($invite->email);
        $row[] = $expired ? t('Expired') : t('Pending');
        $row[] = l(t('withdraw'), "invite/withdraw/$invite->reg_code", array('query' => drupal_get_destination())) .
          ($expired ? ' | ' . l(t('resend'), "invite/resend/$invite->reg_code") : '');
        break;
    }
    $rows[] = $row;
  }

  return theme('invite_user_overview', array('items' => $rows));
}

/**
 * Theme function; display the invite overview table.
 *
 * @param $items
 *   An array of table rows.
 *
 * @ingroup themeable
 */
function theme_invite_user_overview($variables) {
  $items = $variables['items'];
  if (count($items) > 0) {
    $headers = array(t('E-mail'), t('Status'), t('Operations'));
    $output  = theme('table', array('header' => $headers, 'rows' => $items, 'attributes' => array('id' => 'invites')));
    $output .= theme('pager');
  }
  else {
    $output = t('No invitations available.');
  }

  return $output;
}
