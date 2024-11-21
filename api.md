以下是基于您提供的代码编写的 API 文档：

---

## **用户登录 API**

### **接口描述**
用户通过此接口进行登录验证，成功后返回一个 JWT Token。

### **请求方式**
`POST`

### **接口地址**
`/newWeb/login`

### **请求参数**
| 参数名      | 类型    | 是否必填 | 描述                 |
|-------------|---------|----------|----------------------|
| username    | string  | 是       | 用户名               |
| password    | string  | 是       | 密码                 |

### **响应参数**
| 字段名      | 类型    | 描述                       |
|-------------|---------|----------------------------|
| message     | string  | 响应信息                  |
| token       | string  | 用户认证的 JWT Token，失败时为空 |

### **状态码**
| 状态码 | 描述                         |
|--------|------------------------------|
| 200    | 登录成功                     |
| 400    | 缺少用户名或密码             |
| 401    | 用户不存在或密码错误         |
| 405    | 非 POST 方法                 |

### **示例**
#### **请求**
```json
{
    "username": "john_doe",
    "password": "123456"
}
```

#### **成功响应**
```json
{
    "message": "Login successful",
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
}
```

#### **失败响应**
- 用户不存在：
  ```json
  {
      "message": "user does not exist",
      "token": ""
  }
  ```
- 密码错误：
  ```json
  {
      "message": "Incorrect password",
      "token": ""
  }
  ```

---

## **用户注册 API**

### **接口描述**
用户通过此接口注册新账号。

### **请求方式**
`POST`

### **接口地址**
`/newWeb/register`

### **请求参数**
| 参数名      | 类型    | 是否必填 | 描述                          |
|-------------|---------|----------|-------------------------------|
| username    | string  | 是       | 用户名                        |
| password    | string  | 是       | 密码（建议加密存储）          |
| email       | string  | 是       | 邮箱                          |
| phone       | string  | 是       | 电话                          |
| age         | int     | 是       | 年龄                          |
| gender      | string  | 是       | 性别                          |
| idCard      | string  | 是       | 身份证号                      |

### **响应参数**
| 字段名      | 类型    | 描述                       |
|-------------|---------|----------------------------|
| message     | string  | 响应信息                  |
| userId      | int     | 成功注册的用户 ID，失败时为空 |

### **状态码**
| 状态码 | 描述                          |
|--------|-------------------------------|
| 201    | 注册成功                      |
| 400    | 缺少必要参数或用户名已存在    |
| 405    | 非 POST 方法                  |
| 500    | 用户注册失败（内部错误）      |

### **示例**
#### **请求**
```json
{
    "username": "john_doe",
    "password": "123456",
    "email": "john_doe@example.com",
    "phone": "1234567890",
    "age": 30,
    "gender": "male",
    "idCard": "123456789012345678"
}
```

#### **成功响应**
```json
{
    "message": "User registered successfully",
    "userId": 1
}
```

#### **失败响应**
- 参数缺失：
  ```json
  {
      "message": "Missing or invalid parameters",
      "userId": ""
  }
  ```
- 用户名已存在：
  ```json
  {
      "message": "Username already exists",
      "userId": ""
  }
  ```
- 注册失败（内部错误）：
  ```json
  {
      "message": "Failed to register user",
      "userId": ""
  }
  ```

---

### **注意事项**
1. **安全性**：
   - 登录和注册接口建议在生产环境中使用 HTTPS 保护用户数据。
   - 密码需进行加密存储，可使用如 bcrypt 的加密算法。
2. **Token 使用**：
   - Token 的有效期为 1 小时（3600 秒），到期需重新登录获取。
3. **输入校验**：
   - 后端应对用户输入进行严格校验，避免 SQL 注入和 XSS 攻击。
4. **扩展性**：
   - 可添加更多字段，如用户角色、权限等。

希望这些文档能帮助您进行接口的开发和测试！