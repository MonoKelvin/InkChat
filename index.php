<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>InkChat API</title>
</head>

<body>
    InkChat Api
    <ul>
        <li>
            登录验证：
            <form action="./api/user/login.php" method="post">
                <div>
                    <select name="verify_method">
                        <option value="id">id</option>
                        <option value="password">password</option>
                    </select>
                    <input name="id" type="number" value="1">
                    <input name="account" type="text" value="15007083506@qq.com">
                    <input name="password" type="password" value="123456">
                    <button type="submit">测试</button>
                </div>
            </form>
        </li>
        <li>
            查询用户信息：
            <form action="./api/user/user.php" method="post">
                <div>
                    <input name="id" type="number" value="1">
                    <button type="submit">测试</button>
                </div>
            </form>
        </li>
        <li>
            获取图片：
            <form action="./api/image.php" method="get">
                <div>
                    <input name="user" type="text" value="user">
                    <input name="type" type="text" value="avatar">
                    <input name="id" type="number" value="1">
                    <button type="submit">测试</button>
                </div>
            </form>
        </li>
        <li>
            注册验证：
            <form action="./api/signup.php" method="post">
                <div>
                    <input name="nickname" type="number" value="nickname">
                    <input name="account" type="text" value="15007083506@qq.com">
                    <input name="password" type="password" value="">
                    <input name="repassword" type="password" value="">
                    <button type="submit">测试</button>
                </div>
            </form>
        </li>
    </ul>
</body>

</html>
