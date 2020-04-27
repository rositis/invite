<?php

/**
 * @file
 * File for Admin.
 */

/**
 * Settings form for Invite By Mail.
 *
 * @return array
 *   Return array with system settings form.
 */
function invite_by_email_settings_form() {
  // Get the full config object
  $config = config('invite_by_email.settings');
  $form['#config'] = 'invite_by_email.settings';

  $form['invite_message_editable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Editable subject'),
    '#description' => t('Choose whether users should be able to customize the subject.'),
    // @codingStandardsIgnoreLine
    '#default_value' => config_get('invite_by_email.settings','invite_message_editable'),
  );

  $form['invite_default_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    // @codingStandardsIgnoreLine
    '#default_value' => $config->get('invite_default_mail_subject'),
    '#description' => t('Type the default subject of the invitation e-mail.') . ' ' . t('Use the syntax [token] if you want to insert a replacement pattern.'),
    '#required' => TRUE,
  );

  // Display the list of available placeholders if token module is installed.
  if (module_exists('token')) {
    $form['auto_entitylabel']['token_help'] = array(
      '#theme' => 'token_tree',
      '#dialog' => TRUE,
    );
  }

  $form['invite_default_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail template'),
    // @codingStandardsIgnoreLine
    '#default_value' => $config->get('invite_default_mail_body'),
    '#required' => TRUE,
    '#description' => t('Use the syntax [token] if you want to insert a replacement pattern.'),
  );

  $form['invite_default_replace_tokens'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply token replacements'),
     // @codingStandardsIgnoreLine
    '#default_value' => $config->get('invite_default_replace_tokens'),
    '#description' => t('Whether token replacement patterns should be applied.'),
  );

  if (module_exists('token')) {
    $form['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    try {
      $token_help_widget = theme('token_tree', array(
        'token_types' => array(
          'user',
          'profile',
          'invite',
        ),
      ));
    }
    catch (Exception $e) {
      // TODO: What should I put here as a default?
      $token_help_widget = '';
    }

    $form['token_help']['help'] = array(
      '#markup' => $token_help_widget,
    );
  }

  $form['invite_use_users_email'] = array(
    '#type' => 'radios',
    '#title' => t('<em>From</em> e-mail address'),
    '#description' => t('Choose which e-mail address will be in the From: header for the invitation mails sent; <em>site</em> or <em>inviter</em>. <em>Site</em> will use the default e-mail address of the site, whereas <em>inviter</em> will use the e-mail address of the user who is sending the invitation. Alternatively, you can set this value manually by clicking on <em>advanced settings</em> below.'),
    '#options' => array(t('Site'), t('Inviter')),
    // @codingStandardsIgnoreLine
    '#default_value' => $config->get('invite_use_users_email'),
  );

  $form['invite_use_users_email_replyto'] = array(
    '#type' => 'radios',
    '#title' => t('<em>Reply-To</em> e-mail address'),
    '#description' => t('Choose which e-mail address will be in the Reply-To: header for the invitation mails sent; <em>site</em> or <em>inviter</em>. <em>Site</em> will use the default e-mail address of the site, whereas <em>inviter</em> will use the e-mail address of the user who is sending the invitation. Alternatively, you can set this value manually by clicking on <em>advanced settings</em> below.'),
    '#options' => array(t('Site'), t('Inviter')),
    // @codingStandardsIgnoreLine
    '#default_value' => $config->get('invite_use_users_email_replyto'),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('<strong>Note:</strong> The addresses defined here will replace the site e-mail, if it is selected above.'),
  );

  $form['advanced']['invite_manual_from'] = array(
    '#type' => 'textfield',
    '#title' => t('Manually override <em>From</em> e-mail address'),
    // @codingStandardsIgnoreLine
    '#default_value' => $config->get('invite_manual_from'),
    '#description' => t('The e-mail address the invitation e-mail is sent from.'),
  );

  $form['advanced']['invite_manual_reply_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Manually override <em>Reply-To</em> e-mail address'),
    // @codingStandardsIgnoreLine
    '#default_value' => $config->get('invite_manual_reply_to'),
    '#description' => t('The e-mail address you want recipients to reply to.'),
  );

  return system_settings_form($form);

}
