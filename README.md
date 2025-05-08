以下是基于你提供的表结构设计的 API 文档，每个接口的状态码均返回 `200`，响应报文包  含 `"code"`, `"message"`, 和 `"data"` 字段，统一接口规范如 下。

---

### **1. 用户管理接口**

#### **1.1 用户注册**
**描述**: 用户注册。  
**URL**: `/api/users/register`  
**方法**: `POST`  
**请求体**:
```json
{
  "username": "User",
  "password": "123456",
  "email": "user@example.com",
  "phone": "1234567890",
  "age": 25,
  "gender": "M",
  "idNumber": "123456789012345678",
  "userLevel": 1
}
```
**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Registration successful",
  "data": {
    "id": 1,
    "username": "User",
    "email": "user@example.com"
  }
}
```
失败:
```json
{
  "code": 200,
  "message": "Registration failed: Email already exists",
  "data": null
}
```

---

#### **1.2 用户登录**
**描述**: 用户登录验证。  
**URL**: `/api/users/login`  
**方法**: `POST`  
**请求体**:
```json
{
  "email": "user@example.com",
  "password": "123456"
}
```
**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Login successful",
  "data": {
    "id": 1,
    "username": "User",
    "email": "user@example.com",
    "role": "学生"
  }
}
```
失败:
```json
{
  "code": 200,
  "message": "Login failed: Invalid email or password",
  "data": null
}
```

---

### **2. 航班管理接口**

#### **2.1 查询航班信息**
**描述**: 根据条件查询航班信息。  
**URL**: `/api/flights/search`  
**方法**: `GET`  
**请求参数** (Query 参数):
- `departure` (可选): 出发地
- `destination` (可选): 目的地
- `departureTime` (可选): 出发时间范围（时间戳，秒级）

**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Search successful",
  "data": [
    {
      "id": 101,
      "flightNumber": "MU1234",
      "departure": "Beijing",
      "destination": "Shanghai",
      "departureTime": 1712345678,
      "arrivalTime": 1712355678,
      "firstClassSeats": 10,
      "economyClassSeats": 50,
      "firstClassPrice": 1000.0,
      "economyClassPrice": 500.0,
      "airlineCompany": "China Eastern Airlines"
    }
  ]
}
```
失败:
```json
{
  "code": 200,
  "message": "Search failed: No flights found",
  "data": null
}
```

---

#### **2.2 创建航班**
**描述**: 添加新航班。  
**URL**: `/api/flights/create`  
**方法**: `POST`  
**请求体**:
```json
{
  "flightNumber": "MU1234",
  "departure": "Beijing",
  "destination": "Shanghai",
  "departureTime": 1712345678,
  "arrivalTime": 1712355678,
  "firstClassSeats": 10,
  "economyClassSeats": 50,
  "firstClassPrice": 1000.0,
  "economyClassPrice": 500.0,
  "airlineCompany": "China Eastern Airlines",
  "aircraftModel": "A320"
}
```
**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Flight created successfully",
  "data": {
    "id": 101,
    "flightNumber": "MU1234"
  }
}
```
失败:
```json
{
  "code": 200,
  "message": "Flight creation failed: Duplicate flight number",
  "data": null
}
```

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
  "code": 200,
  "message": "Order creation failed: Flight fully booked",
  "data": null
}
```

---

#### **3.2 查询订单**
**描述**: 根据用户 ID 查询订单信息。  
**URL**: `/api/orders/search`  
**方法**: `GET`  
**请求参数** (Query 参数):
- `userId`: 用户 ID  

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
  "code": 200,
  "message": "Query failed: No orders found",
  "data": null
}
```

---

#### **3.3 更新订单状态**
**描述**: 更新订单状态（如支付、退款）。  
**URL**: `/api/orders/update`  
**方法**: `PUT`  
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
  "code": 200,
  "message": "Order update failed: Invalid order ID",
  "data": null
}
```

---

### 统一说明
- **状态码**: 所有接口均返回 HTTP 状态码 `200`。
- **响应报文格式**:
  - `code`: 状态码，固定为 `200`。
  - `message`: 描述信息，指明成功或失败的原因。
  - `data`: 包含具体返回数据，失败时为 `null`。
