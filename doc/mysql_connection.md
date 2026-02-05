

# mysql connection连接数据库

### 获取mysql驱动

```cpp
sql::mysql::MySQL_Driver *driver = sql::mysql::get_driver_instance();
```

### 连接数据库

```cpp
sql::Connection* connection = driver->connect(_config["host"], _config["user"], _config["password"]);
```

### 设置数据库

```cpp
connection->setSchema(_config["database"]);
```

### 设置连接超时为当前时间戳，单位为秒

```cpp
auto currentTime = std::chrono::system_clock::now().time_since_epoch();
long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
connection->setClientOption("connectTimeout", &timestamp);
```

### 释放连接

```cpp
connection->close();
```

## 建表

```cpp
//sql::Connection* connection;
sql::Statement *stmt = connection->createStatement();
stmt->execute("CREATE TABLE IF NOT EXISTS `user` (`id` int(11) NOT NULL AUTO_INCREMENT,`name` varchar(255) NOT NULL,`password` varchar(255) NOT NULL,`create_time` varchar(20) NOT NULL,PRIMARY KEY (`id`)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;");
delete stmt;
```

## Insert

```cpp
sql::Statement *stmt = _connection->createStatement();
int pass = atoi(password.c_str());
std::string new_pass = std::to_string(pass);
// 获取当前时间并格式化为 YY-MM-DD
auto currentTime = std::chrono::system_clock::now();
std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
std::tm *tm = std::localtime(&currentTimeT);
std::ostringstream oss;
oss << std::put_time(tm, "%Y-%m-%d"); // 使用 %Y-%m-%d 以确保四位数的年份
std::string formattedDate = oss.str();
stmt->execute("INSERT INTO `user` (`name`, `password`, `create_time`) VALUES ('" + name + "', '" + new_pass + "', '" + formattedDate + "');");
delete stmt;
```

## Delete

```cpp
sql::Statement *stmt = _connection->createStatement();
stmt->execute("DELETE FROM `user` WHERE `name` = '" + name + "'");
delete stmt;
```

## Update

```cpp
sql::Statement *stmt = _connection->createStatement();
int pass = ~(atoi(password.c_str()));
std::string new_pass = std::to_string(pass);
stmt->execute("UPDATE `user` SET `password` = '" + new_pass + "' WHERE `name` = '" + name + "'");
delete stmt;
```

## Select

```cpp
sql::Statement *stmt = _connection->createStatement();
sql::ResultSet *res = stmt->executeQuery("SELECT * FROM `user`");
while (res->next())
{
    std::cout << "id: " << res->getInt("id") << ", name: " << res->getString("name") << ", password: " << res->getString("password") << ", create_time: " <<  res->getString("create_time") << std::endl;
}
delete res;
delete stmt;
```

## 调用存储过程

```cpp
sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
sql::Connection *con = driver->connect(url_, user_, pass_);
con->setSchema(schema_);
std::unique_ptr<sql::PreparedStatement> stmt(con->_con->prepareStatement("CALL reg_user(?,?,?,@result)"));
// 设置输入参数
stmt->setString(1, name);
stmt->setString(2, email);
stmt->setString(3, pwd);
// 执行存储过程
stmt->execute();
// 如果存储过程设置了会话变量或有其他方式获取输出参数的值，你可以在这里执行SELECT查询来获取它们
// 例如，如果存储过程设置了一个会话变量@result来存储输出结果，可以这样获取：
std::unique_ptr<sql::Statement> stmtResult(con->_con->createStatement());
std::unique_ptr<sql::ResultSet> res(stmtResult->executeQuery("SELECT @result AS result"));
if (res->next())
{
     int result = res->getInt("result");
     std::cout << "Result: " << result << std::endl;
     pool_->returnConnection(std::move(con));
     std::cout << "RegUser Success result is " << result << std::endl;
     return result;
}
```

## Select

```cpp
auto con = pool_->getConnection();//拿到数据库连接
// 准备查询语句
std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("SELECT email FROM user WHERE name = ?"));
// 绑定参数
pstmt->setString(1, name);
// 执行查询
std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
// 遍历结果集
while (res->next())
{
    std::cout << "Check Email: " << res->getString("email") << std::endl;
    if (email != res->getString("email"))
    {
             pool_->returnConnection(std::move(con));
             return false;
    }
     pool_->returnConnection(std::move(con));
     return true;
}
```



## Update

```cpp
auto con = pool_->getConnection();
// 准备查询语句
std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("UPDATE user SET pwd = ? WHERE name = ?"));
// 绑定参数
pstmt->setString(2, name);
pstmt->setString(1, newpwd);
// 执行更新
int updateCount = pstmt->executeUpdate();
```

## Insert

```cpp
auto con = pool_->getConnection();
// 准备SQL语句
std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("INSERT INTO friend_apply (from_uid, to_uid) values (?,?) ON DUPLICATE KEY UPDATE from_uid = from_uid, to_uid = to_uid"));
pstmt->setInt(1, from); // from id
pstmt->setInt(2, to);
// 执行更新
int rowAffected = pstmt->executeUpdate();
if (rowAffected < 0)
{
        return false;
}
return true;
```

# QT连接Mysql

* 头文件：<QSqlDatabase>

## 连接数据库

```cpp
	//添加mysql驱动
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    // 2. 设置连接参数
    db.setHostName("127.0.0.1");      // 数据库 IP 地址
    db.setPort(3306);                 // 端口号，默认 3306
    db.setDatabaseName("test");    // 数据库名称
    db.setUserName("root");           // 用户名
    db.setPassword("Liao20040504");         // 密码

    // 3. 尝试打开连接
    if (!db.open()) {
        qDebug() << "连接失败：" << db.lastError().text();
        return;
    }

    qDebug() << "成功连接到 MySQL 数据库！";
```

## 1.基础操作：使用 `QSqlQuery`

`QSqlQuery` 是最直接的操作方式，适用于执行标准的 SQL 语句。

### 增、删、改 (Insert, Update, Delete)

对于这些操作，我们通常关注执行是否成功以及受影响的行数

```cpp
QSqlQuery query;

// --- 插入数据 ---
if (!query.exec("INSERT INTO users (name, age) VALUES ('Alice', 25)")) {
    qDebug() << "Insert error:" << query.lastError().text();
}

// --- 更新数据 ---
query.exec("UPDATE users SET age = 26 WHERE name = 'Alice'");

// --- 删除数据 ---
query.exec("DELETE FROM users WHERE age > 30");

qDebug() << "最后一次操作影响了" << query.numRowsAffected() << "行";
```

### 查 (Select)

查询需要通过 `next()` 循环遍历结果集。

```cpp
if (query.exec("SELECT id, name, age FROM users")) {
    while (query.next()) {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        int age = query.value(2).toInt(); // 也可以用索引
        qDebug() << id << name << age;
    }
}
```

## 2.进阶：使用预处理（防止 SQL 注入）

**永远不要**直接用字符串拼接 SQL 语句！使用占位符不仅更安全，而且在批量处理时效率更高。

```cpp
QSqlQuery query;
query.prepare("INSERT INTO users (name, age) VALUES (:name, :age)");

// 绑定数据
query.bindValue(":name", "Bob");
query.bindValue(":age", 22);

if (!query.exec()) {
    qDebug() << "Error:" << query.lastError().text();
}
```

## 3.高级：SQL Model / View 架构

不需要写任何 SQL 语句，就能让数据库数据直接显示在界面上，并且实现**所见即所得**的编辑。

### QSqlTableModel (单表模型)

适用于对单张表进行显示和编辑。

```cpp
#include <QSqlTableModel>
#include <QTableView>

// 1. 创建模型
QSqlTableModel *model = new QSqlTableModel(this);
model->setTable("users");
model->setEditStrategy(QSqlTableModel::OnFieldChange); // 修改界面即同步到数据库
model->select(); // 查询数据

// 2. 绑定到视图（如 QTableView）
ui->tableView->setModel(model);

// 3. 过滤与排序
model->setFilter("age > 20");
model->setSort(1, Qt::DescendingOrder); // 按第2列降序
model->select();
```

### QSqlQueryModel (只读模型)

如果你需要执行复杂的关联查询（如 `JOIN`），但不需要在表格里直接修改数据，用这个。

```cpp
QSqlQueryModel *model = new QSqlQueryModel;
model->setQuery("SELECT a.name, b.dept_name FROM users a JOIN departments b ON a.d_id = b.id");
ui->tableView->setModel(model);
```

### QSqlRelationalTableModel (关系模型)

这是最强大的模型，它可以将数据库中的“外键 ID”自动转换成“名称”显示。

- **场景：** 员工表里存的是 `dept_id` (1, 2, 3)，但你想让用户在表格里看到“开发部”、“人事部”。

```cpp
QSqlRelationalTableModel *model = new QSqlRelationalTableModel;
model->setTable("users");

// 将第3列（dept_id）映射到 departments 表的 id 字段，并显示其 name 字段
model->setRelation(2, QSqlRelation("departments", "id", "name"));
model->select();

ui->tableView->setModel(model);
// 设置下拉框代理，让用户修改时直接选部门名字
ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
```

| **工具**                     | **适用场景**                  | **优点**                  |
| ---------------------------- | ----------------------------- | ------------------------- |
| **QSqlQuery**                | 简单的 SQL 逻辑、后台数据处理 | 灵活、直接、开销小        |
| **QSqlTableModel**           | 需要在表格中管理单张表        | 零 SQL 编程、支持自动同步 |
| **QSqlRelationalTableModel** | 需要处理外键转换的 UI 界面    | 用户友好、逻辑自动封装    |

## 执行事务

```cpp
if (db.driver()->hasFeature(QSqlDriver::Transactions)) {
    db.transaction(); // 开始事务
    QSqlQuery query;
    if (query.exec("...") && query.exec("...")) {
        db.commit(); // 提交
    } else {
        db.rollback(); // 回滚
    }
}
```

## Delegate代理/委托

在 Qt 的 Model/View 架构中，**Delegate（委托）** 是负责“渲染”和“编辑”数据的核心组件。

默认情况下，`QSqlTableModel` 只是简单地把数据显示为文本。如果你希望在表格里显示一个**下拉框（ComboBox）**、**日期选择器**、**星星评分**或者**开关按钮**，你就必须自定义 Delegate。

------

### 1. 为什么要用 Delegate？

- **控制外观**：比如根据数值大小改变背景颜色。
- **限制输入**：用户只能从下拉列表中选，不能乱填。
- **优化体验**：点击单元格直接弹出日历。

----------------------------------------------------------------------------------

### 2. 自定义 Delegate 的核心步骤

你需要继承 `QStyledItemDelegate` 并重写四个关键函数：

| **函数名**                   | **作用**                              |
| ---------------------------- | ------------------------------------- |
| **`createEditor()`**         | 创建用于编辑的控件（如 `QSpinBox`）。 |
| **`setEditorData()`**        | 将 Model 中的数据填入编辑器。         |
| **`setModelData()`**         | 将编辑器里的新值写回 Model。          |
| **`updateEditorGeometry()`** | 确保编辑器的大小和单元格对齐。        |

------------------------------------------------------------------------------------------------------------------

### 3. 实战代码：在数据库表格中嵌入下拉框

假设你的数据库里有一列“性别”，你希望用户只能选择“男”或“女”。

```cpp
#include <QStyledItemDelegate>
#include <QComboBox>

class SexDelegate : public QStyledItemDelegate {
public:
    // 1. 创建下拉框编辑器
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override {
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("男");
        editor->addItem("女");
        return editor;
    }

    // 2. 将数据从 Model 传给 Editor
    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(value);
    }

    // 3. 将修改后的数据传回 Model
    void setModelData(QWidget *editor, const QModelIndex &index,
                      QAbstractItemModel *model) const override {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }

    // 4. 更新编辑器几何位置
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override {
        editor->setGeometry(option.rect);
    }
};
```

--------------------------------------------------------------

### 4. 如何应用到视图（View）？

编写好类后，你只需要一行代码将其应用到指定的列：

```cpp
// 假设第 2 列是“性别”列
ui->tableView->setItemDelegateForColumn(2, new SexDelegate());
```

------------------------------

### 5. 进阶技巧：条件渲染

如果你不只是想改变编辑器，还想改变**非编辑状态**下的样式（比如不及格的分数显示红色），可以重写 `paint()` 函数：

```cpp
void paint(QPainter *painter, const QStyleOptionViewItem &option, 
           const QModelIndex &index) const override {
    int score = index.data().toInt();
    if (score < 60) {
        QStyleOptionViewItem opt = option;
        opt.palette.setColor(QPalette::Text, Qt::red); // 字体变红
        QStyledItemDelegate::paint(painter, opt, index);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
```

