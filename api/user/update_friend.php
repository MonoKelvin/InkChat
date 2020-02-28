<?php
require_once(__DIR__ . '\user.php');

if (@$_POST['uid'] && @$_POST['fid']) {
    $data = [
        'remark' => @$_POST['remark'],
        'top' => @$_POST['top'],
        'subgroup' => @$_POST['subgroup']
    ];
    updateFriendInfo(@$_POST['uid'], @$_POST['fid'], $data);
} else {
    errorReply(404);
}
