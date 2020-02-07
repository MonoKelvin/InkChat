<?php
require_once(__DIR__ . '\..\utility.php');

/**
 * 通过id验证用户
 * @param int $uid 用户ID
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
 * @note uid只是辅助快速查询数据库，账号和密码也会进行匹配。
 */
function verifyLoginById($uid, $account, $password)
{
    if (!$uid || !$account || !$password) {
        reply(null, 700, '用户数据为空');
        die;
    }

    // 查询数据库
    $db = MySqlAPI::getInstance();
    if (!$db) {
        reply(null, 600, '数据库连接失败');
        $db->close();
        die;
    }
    $res = $db->getRow('SELECT * FROM User WHERE ID=' . $uid);
    $db->close();

    if ($res) {
        if ($res['Account'] !== $account || $res['Password'] !== $password) {
            reply(null, 702, '账号或密码错误');
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
        reply(null, 701, '用户不存在');
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
        reply(null, 700, '用户数据为空');
        die;
    }

    // 查询数据库
    $db = MySqlAPI::getInstance();
    if (!$db) {
        reply(null, 600, '数据库连接失败');
        $db->close();
        die;
    }
    $res = $db->getRow("SELECT * FROM User WHERE Account=$account AND Password=$password");
    $db->close();

    if (@$res['Account'] !== $account || @$res['Password'] !== $password) {
        reply(null, 702, '账号或密码错误');
        die;
    }

    // 获取信息
    reply([
        'id' => $res['ID'],
        'nickName' => $res['NickName'],
        'signature' => $res['Signature'],
        'gender' => $res['Gender'],
        'avatar' => $res['Avatar']
    ]);
}
