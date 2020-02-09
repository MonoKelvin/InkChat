<?php
require_once(__DIR__ . '\user.php');

if(@$_POST['id']) {
    getUserFriends(@$_POST['id']);
} else {
    errorReply(404);
}
