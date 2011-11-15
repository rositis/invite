<?php

/**
 * @file
 * Hooks provided by Invite module.
 */

/**
 * Respond to an invitation being sent out.
 *
 * This hook is invoked right after an invitation has been created and sent out.
 *
 * @param $invite
 *   The invite object that has just been created.
 */
function hook_invite_send($invite) {
}

/**
 * Respond to an invitation withdrawal.
 *
 * This hook is invoked right after an invitation has been withdrawn.
 *
 * @param $invite
 *   The invite object that has just been withdrawn.
 */
function hook_invite_withdraw($invite) {
}

/**
 * Respond to an invitation acceptance.
 *
 * This hook is invoked right after a user account has been created by using an
 * invitation.
 *
 * @param $invite
 *   The invite object that has just been accepted.
 * @param $accout
 *   The newly created user account.
 */
function hook_invite_accept($invite, $accout) {
}

/**
 * Alter roles that are being added to the user who has just accepted an invitation
 * and created an account.
 *
 * This hook is invoked after the target roles for the new registrant have been
 * determined, but before they have been assigned to the user. Modify $roles to
 * change what roles will be added to the user.
 *
 * @param $roles
 *   An array of role IDs that will be added to the user.
 * @param $invite
 *   The invite that the user has accepted.
 * @param $account
 *   The new account that is being created. This object comes from hook_user_presave,
 *   and is not a fully populated user object.
 */
function hook_invite_target_roles(&$roles, $invite, $account) {
}

/**
 * Alter the limit of invitations a user is allowed to send.
 *
 * This hook is invoked when the invitation limit is being determined for a
 * specific user. Modules implementing this hook should return the number of
 * invitations they wish the user to be limited to. If multiple modules return
 * values, the last invoked module will take precedence. The hook should
 * retrun INVITE_UNLIMITED if it wishes to remove the limitation.
 *
 * @param $account
 *   The account for which the limit is being determined.
 * @param $limit
 *   The currently exposed limit based on Invite's role settings.
 * @return
 *   The number of invitations that the module wants to limit the user to. If
 *   the module does not wish to alter the limit, it should not return anything.
 */
function hook_invite_limit($account, $limit) {
  if ($account->uid == 1) {
    return 1000;
  }
}
