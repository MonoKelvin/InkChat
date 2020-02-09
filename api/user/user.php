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
    $res = $db->getRow('SELECT * FROM `User` WHERE `ID`=' . $id);
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
    $res = $db->getRow("SELECT * FROM `User` WHERE `Account`='$account' AND `Password`='$password'");
    $db->close();

    if (@$res['Account'] !== $account || @$res['Password'] !== $password) {
        errorReply(702);
        die;
    }

    // 模拟延迟1s
    // sleep(1);

    // 获取信息
    reply([
        'id' => $res['ID'],
        'nickName' => $res['NickName'],
        'signature' => $res['Signature'],
        'gender' => $res['Gender'],
        'avatar' => $res['Avatar']
    ]);
}

/**
 * 查询用户的所有好友信息
 * @param int $id 用户id
 * @return array
 * 数组形式是按好友所在组分的
 * [
 *   {
 *      "subgroup": "groupName",
 *      "members": [
 *           {
 *               "id": 2,
 *               "hostAddress": "192.64.12.7",
 *               "isTop": true,
 *               "gender": "1",
 *               "remark": "我的好友",
 *               "nickName": "MonoKelvin",
 *               "signature": "没有签名...",
 *               "thumb": "../cache/avatar/94f3f838e8c51ac0eac6c74588403c25.png"
 *           },
 *           { }
 *      ]
 *   },
 *   { }
 * ]
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
        'SELECT `FID`,`HostAddress`,`Remark`,`Top`,`NickName`,`Signature`,`Gender`,`Subgroup`
        FROM GetUserFriends
        WHERE `UID`=' . $id
    );
    $db->close();

    $group = [];
    foreach ($res as $_ => $value) {
        $subgroup = $value['Subgroup'];
        unset($value['Subgroup']);

        // 将用户分配到分组中
        for($i = 0; $i < count($group); $i++) {
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
