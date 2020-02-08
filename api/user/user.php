<?php
require_once(__DIR__ . '\..\mysql_api.class.php');

/**
 * 通过id验证用户
 * @param int $id 用户ID
 * @param string $account 账号
 * @param string $password 密码
 * @return 具体返回数据包含
 * [
 *      'account',
 *      'nickName',
 *      'signature',
 *      'gender',
 *      'avatar',
 * ]
 * @note id只是辅助快速查询数据库，账号和密码也会进行匹配。
 */
function verifyLoginById($id, $account, $password)
{
    if (!$id || !$account || !$password) {
        errorReply(700);
        die;
    }

    // 查询数据库
    $db = @MySqlAPI::getInstance();
    if (!$db) {
        errorReply(600);
        $db->close();
        die;
    }
    $res = $db->getRow('SELECT * FROM User WHERE ID=' . $id);
    $db->close();

    if ($res) {
        if ($res['Account'] !== $account || $res['Password'] !== $password) {
            errorReply(702);
            die;
        }

        // 获取信息
        reply([
            'account' => $res['Account'],
            'nickName' => $res['NickName'],
            'signature' => $res['Signature'],
            'gender' => $res['Gender'],
            'avatar' => $res['Avatar']
        ]);
    } else {
        errorReply(701);
    }
}

/**
 * 通过账号密码验证用户
 * @param string $account 账号
 * @param string $password 密码
 * @return 具体返回数据包含
 * [
 *      'id',
 *      'nickName',
 *      'signature',
 *      'gender',
 *      'avatar'
 * ]
 */
function verifyLoginByPassword($account, $password)
{
    if (!$account || !$password) {
        errorReply(700);
        die;
    }

    // 查询数据库
    $db = @MySqlAPI::getInstance();
    if (!$db) {
        errorReply(600);
        $db->close();
        die;
    }
    $res = $db->getRow("SELECT * FROM User WHERE Account='$account' AND Password='$password'");
    $db->close();

    if (@$res['Account'] !== $account || @$res['Password'] !== $password) {
        errorReply(702);
        die;
    }

    // 模拟延迟1s
    sleep(1);

    // 获取信息
    reply([
        'id' => $res['ID'],
        'nickName' => $res['NickName'],
        'signature' => $res['Signature'],
        'gender' => $res['Gender'],
        'avatar' => $res['Avatar']
    ]);
}
