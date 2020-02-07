<?php
require_once(__DIR__ . '\user.php');

// 通过id查询用户
if (@$_POST['verify_method'] === 'id') {
    verifyLoginById(@$_POST['id'], @$_POST['account'], @$_POST['password']);
} else if (@$_POST['verify_method'] === 'password') {
    verifyLoginByPassword(@$_POST['account'], @$_POST['password']);
} else {
    errorReply(404);
}
