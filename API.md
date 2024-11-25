
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
  "code": 500,
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
  "code": 500,
  "message": "Login failed: Invalid email or password",
  "data": null
}
```

---

### **2. 航班管理接口**

### 修改后的查询航班信息接口设计

**描述**: 根据出发地和到达地查询所有符合条件的航班，不包含已经过期的航班。  
**URL**: `/api/flights/search`  
**方法**: `POST`  
**请求体**:
```json
{
  "departure": "Beijing",
  "destination": "Shanghai"
}
```

**响应**:  
**成功**:
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
    },
    {
      "id": 102,
      "flightNumber": "CA5678",
      "departure": "Beijing",
      "destination": "Shanghai",
      "departureTime": 1713345678,
      "arrivalTime": 1713355678,
      "firstClassSeats": 8,
      "economyClassSeats": 60,
      "firstClassPrice": 1200.0,
      "economyClassPrice": 600.0,
      "airlineCompany": "Air China"
    }
  ]
}
```

**失败**:
```json
{
  "code": 500,
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
  "code": 500,
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
