<?php


class InviteController extends EntityPlusController {

  /**
   * @param array $values
   *
   * @return mixed
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'created' => REQUEST_TIME,
      'expiry' => REQUEST_TIME + config_get('invite.settings','invite_default_expiry_time') * 24 * 60 * 60,
      'uid' => $user->uid,
      'invitee' => 0,
      'status' => 1,
      'reg_code' => _invite_generate_code(),
      'data' => array(),
    );
    return parent::create($values);
  }

  /**
   * @param Invite $entity
   * @param string $view_mode
   * @param null $langcode
   * @param array $content
   * @return mixed
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $account = user_load($entity->uid);
    try {
      $formatted_user = theme('username', array('account' => $account));
    }
    catch (Exception $e) {
      $formatted_user = $account->username;
      watchdog('invite', $e->getMessage());
    }
    $content['author'] = array(
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#markup' => t('Created by: <strong>!author</strong>', array('!author' => $formatted_user)),
    );
    $content['status'] = array(
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#markup' => t('Status: <strong>!status</strong>', array('!status' => $entity->status(TRUE))),
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}
