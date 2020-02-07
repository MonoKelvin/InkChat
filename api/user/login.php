<?php
require_once(__DIR__ . '\user.php');

// 通过id查询用户
if (@$_GET['verify_method'] === 'id') {
    verifyLoginById(@$_POST['uid'], @$_POST['account'], @$_POST['password']);
} else if (@$_GET['verify_method'] === 'password') {
    verifyLoginByPassword(@$_POST['account'], @$_POST['password']);
} else {
    reply(null, 404, '服务器找不到请求的网页');
}
