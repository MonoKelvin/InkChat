<?php
require_once(__DIR__ . '\..\mysql_api.class.php');

/**
 * 通过id验证用户
 * @param int $id 用户ID
 * @param string $account 账号
 * @param string $password 密码
 * @return 之返回成功信息
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
    $res = $db->getRow('SELECT * FROM `User` WHERE `ID`=' . $id);

    if ($res) {
        if ($res['Account'] !== $account || $res['Password'] !== $password) {
            $db->close();
            errorReply(702);
            die;
        }

        $res['Friends'] = $db->getAll(
            'SELECT `id`,`md5`,`hostAddress`,`remark`,`top`,`nickName`,
            `signature`,`gender`,`subgroup`
            FROM GetUserFriends
            WHERE `uid`=' . $id
        );

        reply([
            'id' => $res['ID'],
            'md5' => $res['MD5'],
            'nickName' => $res['NickName'],
            'signature' => $res['Signature'],
            'gender' => $res['Gender'],
            'hostAddress' => $res['HostAddress'],
            'friends' => $res['Friends'],
        ]);
    } else {
        errorReply(701);
    }

    $db->close();
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
    $res = $db->getRow("SELECT * FROM `User` WHERE `Account`='$account' AND `Password`='$password'");

    if (@$res['Account'] !== $account || @$res['Password'] !== $password) {
        $db->close();
        errorReply(702);
        die;
    }

    $res['Friends'] = $db->getAll(
        'SELECT `id`,`hostAddress`,`remark`,`top`,`nickName`,`md5`,`signature`,`gender`,`subgroup`
        FROM GetUserFriends
        WHERE `uid`=' . $res['ID']
    );

    $db->close();

    // 模拟延迟1s
    // sleep(1);

    reply([
        'id' => $res['ID'],
        'md5' => $res['MD5'],
        'nickName' => $res['NickName'],
        'signature' => $res['Signature'],
        'gender' => $res['Gender'],
        'hostAddress' => $res['HostAddress'],
        'friends' => $res['Friends'],
    ]);
}


/**
 * 查询用户的所有好友信息
 * @param int $id 用户id
 * @return array 直接返回每个好友对象
 * @see getUsersFriendsForGroup
 */
function getUserFriends($id)
{
    // 查询数据库
    $db = @MySqlAPI::getInstance();
    if (!$db) {
        errorReply(600);
        $db->close();
        die;
    }
    $res = $db->getAll(
        'SELECT `id`,`md5`,`hostAddress`,`remark`,`top`,`nickName`,`signature`,`gender`,`subgroup`
        FROM GetUserFriends
        WHERE `uid`=' . $id
    );
    $db->close();

    reply($res);
}


/**
 * 查询用户的所有好友信息，得到以组为形式的json对象数组
 * @param int $id 用户id
 * @return array 数组形式是按好友所在组分的
 * @see getUsersFriends
 */
function getUserFriendsForGroup($id)
{
    // 查询数据库
    $db = @MySqlAPI::getInstance();
    if (!$db) {
        errorReply(600);
        $db->close();
        die;
    }
    $res = $db->getAll(
        'SELECT `id`,`md5`,`hostAddress`,`remark`,`top`,`nickName`,`signature`,`gender`,`subgroup`
        FROM GetUserFriends
        WHERE `uid`=' . $id
    );
    $db->close();

    $group = [];
    foreach ($res as $_ => $value) {
        $subgroup = $value['subgroup'];
        unset($value['subgroup']);

        // 将用户分配到分组中
        for ($i = 0; $i < count($group); $i++) {
            if ($group[$i]['subgroup'] === $subgroup) {
                array_push($group[$i]['member'], $value);
                break;
            }
        }

        // 没有那个分组则新建，再分配
        if ($i === count($group)) {
            array_push($group, ['subgroup' => $subgroup, 'member' => [$value]]);
        }
    }

    reply($group);
}
