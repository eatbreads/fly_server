

### 用户 API 文档

以下是针对你的用户管理 API 的文档，涵盖了各个接口的请求方式、路径、请求参数和返回值格式。

---

#### 1. **用户注册接口**

- **URL**: `/api/users/register`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "username": "string",       // 用户名
      "password": "string",       // 密码
      "email": "string",          // 邮箱
      "phone": "string",          // 电话
      "age": 1,             // 年龄
      "gender": "string",         // 性别
      "idNumber": "string",       // 身份证号码
      "userLevel": 1        // 用户等级
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "Registration successful",
        "data": {
            "id": 1,    // 用户ID
            "username": "string",
            "email": "string"
        }
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "Registration failed: Email already exists",
        "data": null
    }
    ```

---

#### 2. **用户登录接口**

- **URL**: `/api/users/login`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "email": "string",         // 邮箱
      "password": "string"       // 密码
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "Login successful",
        "data": {
            "id": 1,
            "username": "string",
            "email": "string",
            "phone": "string",
            "age": 1,
            "gender": "string",
            "idNumber": "string",
            "userLevel": 1
        }
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "Login failed: Invalid email or password",
        "data": null
    }
    ```

---

#### 3. **根据邮箱更改用户信息接口**

- **URL**: `/api/users/update`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "id": 1,              // 用户ID
      "username": "string",       // 用户名 (可选)
      "email": "string",          // 邮箱 必选
      "phone": "string",          // 电话 (可选)
      "age": 1,             // 年龄 (可选)
      "gender": "F",         // 性别 (可选)
      "idNumber": "string",       // 身份证号码 (可选)
      "userLevel": 1        // 用户等级 (可选)
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "Update successful",
        "data": {
            "id": 1
        }
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "Update failed: User not found",
        "data": null
    }
    ```

---

#### 4. **用户注销接口**

- **URL**: `/api/users/logout`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "email": "string"          // 用户邮箱
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "Logout successful",
        "data": null
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "Logout failed: User not found",
        "data": null
    }
    ```

---

#### 5. **根据 Email 获取用户信息接口**

- **URL**: `/api/users/get`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "email": "string"          // 用户邮箱
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "User retrieved successfully",
        "data": {
            "id": 1,
            "username": "string",
            "email": "string",
            "phone": "string",
            "age": 1,
            "gender": "string",
            "idNumber": "string",
            "userLevel": 1
        }
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "User not found",
        "data": null
    }
    ```

---



---

## API 文档：航班管理系统

### 1. **搜索航班信息接口** (`/api/flights/search`)

#### 描述：
通过出发地和目的地搜索航班，返回符合条件的航班列表。

#### 请求方式：
`POST`

#### 请求头：
- `Content-Type: application/json`

#### 请求体：
```json
{
  "departure": "北京",
  "destination": "上海"
}
```

#### 请求参数说明：
| 参数         | 类型     | 必填 | 描述       |
|--------------|----------|------|------------|
| departure    | string   | 是   | 出发地     |
| destination  | string   | 是   | 目的地     |

#### 响应体：
```json
{
  "code": 200,
  "message": "Search successful",
  "data": [
    {
      "id": 1,
      "flightNumber": "CA123",
      "departure": "北京",
      "destination": "上海",
      "departureTime": 1674532500,
      "arrivalTime": 1674540000,
      "firstClassSeats": 10,
      "economyClassSeats": 150,
      "firstClassPrice": 1500,
      "economyClassPrice": 500,
      "airlineCompany": "中国国际航空公司"
    }
  ]
}
```

#### 响应参数说明：
| 参数              | 类型      | 描述                               |
|-------------------|-----------|------------------------------------|
| code              | int       | 响应状态码                         |
| message           | string    | 响应信息                           |
| data              | array     | 符合条件的航班信息列表             |
| id                | int       | 航班 ID                            |
| flightNumber      | string    | 航班号                             |
| departure         | string    | 出发地                             |
| destination       | string    | 目的地                             |
| departureTime     | int       | 出发时间（UNIX 时间戳）           |
| arrivalTime       | int       | 到达时间（UNIX 时间戳）           |
| firstClassSeats   | int       | 头等舱余量                         |
| economyClassSeats | int       | 经济舱余量                         |
| firstClassPrice   | int       | 头等舱票价                         |
| economyClassPrice | int       | 经济舱票价                         |
| airlineCompany    | string    | 航空公司                           |

#### 错误响应：
```json
{
  "code": 500,
  "message": "Search failed: No flights found",
  "data": null
}
```

---

### 2. **创建航班信息接口** (`/api/flights/create`)

#### 描述：
用于创建新的航班信息，提供航班的详细信息。

#### 请求方式：
`POST`

#### 请求头：
- `Content-Type: application/json`

#### 请求体：
```json
{
  "flightNumber": "CA123",
  "departure": "北京",
  "destination": "上海",
  "departureTime": 1674532500,
  "arrivalTime": 1674540000,
  "firstClassSeats": 10,
  "economyClassSeats": 150,
  "firstClassPrice": 1500,
  "economyClassPrice": 500,
  "airlineCompany": "中国国际航空公司",
  "aircraftModel": "Boeing 737"
}
```

#### 请求参数说明：
| 参数              | 类型     | 必填 | 描述               |
|-------------------|----------|------|--------------------|
| flightNumber      | string   | 是   | 航班号             |
| departure         | string   | 是   | 出发地             |
| destination       | string   | 是   | 目的地             |
| departureTime     | int      | 是   | 出发时间（UNIX 时间戳） |
| arrivalTime       | int      | 是   | 到达时间（UNIX 时间戳） |
| firstClassSeats   | int      | 是   | 头等舱座位数       |
| economyClassSeats | int      | 是   | 经济舱座位数       |
| firstClassPrice   | int      | 是   | 头等舱票价         |
| economyClassPrice | int      | 是   | 经济舱票价         |
| airlineCompany    | string   | 是   | 航空公司           |
| aircraftModel     | string   | 是   | 飞机型号           |

#### 响应体：
```json
{
  "code": 200,
  "message": "Flight created successfully",
  "data": {
    "id": 1,
    "flightNumber": "CA123"
  }
}
```

#### 响应参数说明：
| 参数              | 类型      | 描述               |
|-------------------|-----------|--------------------|
| code              | int       | 响应状态码         |
| message           | string    | 响应信息           |
| data              | object    | 包含航班 ID 和 航班号的对象 |
| id                | int       | 新创建的航班 ID    |
| flightNumber      | string    | 航班号             |

#### 错误响应：
```json
{
  "code": 500,
  "message": "Flight creation failed: Duplicate flight number",
  "data": null
}
```

---

### 3. **修改航班信息接口** (`/api/flights/update`)

#### 描述：
根据航班 ID 修改航班的详细信息。

#### 请求方式：
`POST`

#### 请求头：
- `Content-Type: application/json`

#### 请求体：
```json
{
  "id": 1,
  "departure": "上海",
  "arrivalTime": 1674630000,
  "firstClassSeats": 8
}
```

#### 请求参数说明：
| 参数              | 类型     | 必填 | 描述               |
|-------------------|----------|------|--------------------|
| id                | int      | 是   | 航班 ID           |
| departure         | string   | 否   | 出发地             |
| arrivalTime       | int      | 否   | 到达时间（UNIX 时间戳） |
| firstClassSeats   | int      | 否   | 头等舱座位数       |

#### 响应体：
```json
{
  "code": 200,
  "message": "Flight updated successfully",
  "data": {
    "id": 1
  }
}
```


#### 错误响应：
```json
{
  "code": 500,
  "message": "Update failed: Flight not found",
  "data": null
}
```

---

### 4. **删除航班信息接口** (`/api/flights/delete`)

#### 描述：
根据航班 ID 删除指定的航班。

#### 请求方式：
`POST`

#### 请求头：
- `Content-Type: application/json`

#### 请求体：
```json
{
  "id": 1
}
```

#### 响应体：

```json
{
  "code": 200,
  "message": "Flight deleted successfully",
  "data": {
    "id": 1
  }
}
```


#### 错误响应：
```json
{
  "code": 500,
  "message": "Delete failed: Flight not found",
  "data": null
}
```

---


---

### **3. 订单管理接口**

#### **3.1 创建订单**
**描述**: 创建航班订单。  
**URL**: `/api/orders/create`  
**方法**: `POST`  
**请求体**:
```json
{
  "userId": 1,
  "flightId": 101,
  "seatType": "Economy",
  "price": 500.0
}
```
**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Order created successfully",
  "data": {
    "id": 201,
    "status": "Pending payment"
  }
}
```
失败:
```json
{
  "code": 500,
  "message": "Order creation failed: Flight fully booked",
  "data": null
}
```

---

#### **3.2 查询订单**
**描述**: 根据用户 ID 查询订单信息。  
**URL**: `/api/orders/search`  
**方法**: `POST`  
**请求体**:
```json
{
  "userId": 1
}
```
**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Query successful",
  "data": [
    {
      "id": 201,
      "flightNumber": "MU1234",
      "seatType": "Economy",
      "price": 500.0,
      "status": "Pending payment",
      "paymentTime": null
    }
  ]
}
```
失败:
```json
{
  "code": 500,
  "message": "Query failed: No orders found",
  "data": null
}
```

---

#### **3.3 更新订单状态**
**描述**: 更新订单状态（如支付、退款）。  
**URL**: `/api/orders/update`  
**方法**: `POST`  
**请求体**:
```json
{
  "orderId": 201,
  "status": "Paid",
  "paymentTime": 1712345678
}
```
**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Order updated successfully",
  "data": {
    "orderId": 201,
    "status": "Paid"
  }
}
```
失败:
```json
{
  "code": 500,
  "message": "Order update failed: Invalid order ID",
  "data": null
}
```

---

### 统一说明
- **HTTP 状态码**: 固定返回 `200`。
- **请求参数**: 均通过请求体传递。
- **响应报文字段**:
  - `"code"`: 业务状态码，`200` 表示成功，`500` 表示失败。
  - `"message"`: 描述信息。
  - `"data"`: 成功时返回具体数据，失败时为 `null`。


明白了，如果不需要传递 `from` 和 `auth_code` 字段，且请求体采用 JSON 格式，下面是更新后的 API 文档。

---

### 1. 发送邮件接口 (`/api/send`)

#### 请求方法：
- **POST**

#### 请求 URL：
- `127.0.0.1:8080/api/send`

#### 请求头：
- `Content-Type: application/json`

#### 请求体（JSON 格式）：
- `to`: 收件人邮箱（**必填**）
- `subject`: 邮件主题（**必填**）
- `body`: 邮件正文（**必填**）

#### 请求示例：

```json
{
    "to": "123456@qq.com",
    "subject": "Test Email",
    "body": "This is a test email from C++ server." 
}
#### 响应体（JSON 格式）：
- `code`: 返回码，`200` 表示成功，`500` 表示失败。
- `message`: 响应消息，提供错误描述或成功信息。
- `data`: 返回数据，如果成功发送邮件，`data` 会是 `null`。

#### 响应示例：
- 成功：
```json
{
  "code": 200,
  "message": "Email sent successfully",
  "data": null
}
```

- 失败（如缺少必要字段）：
```json
{
  "code": 400,
  "message": "Missing required fields (to, subject, body)",
  "data": null
}
```

- 失败（其他错误）：
```json
{
  "code": 500,
  "message": "Error processing the request",
  "data": null
}
```

---
