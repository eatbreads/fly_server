ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'sjh123456'; 
USE mysql;

UPDATE user SET Host = '%' WHERE User = 'root';
UPDATE user SET Grant_priv = 'Y', Super_priv = 'Y' WHERE User = 'root';
FLUSH PRIVILEGES;

-- 创建数据库
CREATE DATABASE IF NOT EXISTS eat;

-- 切换到数据库
USE eat;

-- 创建 users 表
CREATE TABLE IF NOT EXISTS `users` (
    `id` INT(11) NOT NULL AUTO_INCREMENT COMMENT '主键 ID',
    `username` VARCHAR(255) NOT NULL COMMENT '用户名',
    `password` VARCHAR(255) NOT NULL COMMENT '密码',
    `email` VARCHAR(255) DEFAULT NULL COMMENT '邮箱',
    `phone` VARCHAR(20) DEFAULT NULL COMMENT '手机号',
    `age` INT(11) DEFAULT NULL COMMENT '年龄',
    `gender` VARCHAR(5) DEFAULT NULL COMMENT '性别 (M: 男, F: 女)',
    `id_number` VARCHAR(18) DEFAULT NULL COMMENT '身份证号',
    `user_level` INT(11) DEFAULT '0' COMMENT '用户等级',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB AUTO_INCREMENT = 1 DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_general_ci COMMENT = '用户表';

-- Active: 1732536662967@@127.0.0.1@3306@eat
CREATE TABLE `flights` (
    `id` int NOT NULL AUTO_INCREMENT COMMENT '主键 ID',
    `departure` varchar(255) NOT NULL COMMENT '出发地',
    `departure_time` datetime NOT NULL COMMENT '出发时间',
    `destination` varchar(255) NOT NULL COMMENT '目的地',
    `arrival_time` datetime NOT NULL COMMENT '预计到达时间',
    `flight_number` varchar(50) NOT NULL COMMENT '航班号',
    `first_class_seats` int NOT NULL DEFAULT 0 COMMENT '头等舱余量',
    `business_class_seats` int NOT NULL DEFAULT 0 COMMENT '商务舱余量',
    `economy_class_seats` int NOT NULL DEFAULT 0 COMMENT '经济舱余量',
    `first_class_price` int NOT NULL DEFAULT 0 COMMENT '头等舱票价 (单位: 分)',
    `business_class_price` int NOT NULL DEFAULT 0 COMMENT '商务舱票价 (单位: 分)',
    `economy_class_price` int NOT NULL DEFAULT 0 COMMENT '经济舱票价 (单位: 分)',
    `departure_airport` varchar(255) NOT NULL COMMENT '出发机场名',
    `arrival_airport` varchar(255) NOT NULL COMMENT '到达机场名',
    `boarding_gate` varchar(50) DEFAULT NULL COMMENT '进站口',
    `aircraft_model` varchar(255) DEFAULT NULL COMMENT '飞机型号',
    `airline_company` varchar(255) DEFAULT NULL COMMENT '航空公司',
    `has_meal` int NOT NULL DEFAULT 0 COMMENT '有无餐食 (0: 无, 1: 有)',
    `luggage_size_limit` int NOT NULL DEFAULT 0 COMMENT '行李大小限制 (单位: 公斤)',
    `is_international` int NOT NULL DEFAULT 0 COMMENT '是否国际航班 (0: 否, 1: 是)',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB AUTO_INCREMENT = 1 DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_general_ci COMMENT = '航班表';


-- 创建 orders 表
CREATE TABLE `orders` (
    `id` INT NOT NULL AUTO_INCREMENT COMMENT '主键 ID',
    `user_id` INT NOT NULL COMMENT '用户 ID',
    `flight_id` INT NOT NULL COMMENT '航班信息 ID',
    `seat_type` VARCHAR(50) NOT NULL COMMENT '座位类别',
    `price` INT NOT NULL COMMENT '票价',
    `order_status` VARCHAR(20) NOT NULL COMMENT '订单状态',
    `payment_time` DATETIME DEFAULT NULL COMMENT '支付时间',
    `refund_time` DATETIME DEFAULT NULL COMMENT '退款时间',
    PRIMARY KEY (`id`),
    FOREIGN KEY (`user_id`) REFERENCES `users`(`id`) ON DELETE CASCADE,
    FOREIGN KEY (`flight_id`) REFERENCES `flights`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='订单表';

-- 扩充用户数据
INSERT INTO `users` (`username`, `password`, `email`, `phone`, `age`, `gender`, `id_number`, `user_level`)
VALUES
('Alice', 'password123', 'alice@example.com', '12345678901', 25, 'F', '110101199001011234', 1),
('Bob', 'password456', 'bob@example.com', '12345678902', 30, 'M', '110102199002021234', 2),
('Charlie', 'password789', 'charlie@example.com', '12345678903', 22, 'M', '110103199003031234', 0),
('Daisy', 'password111', 'daisy@example.com', '12345678904', 28, 'F', '110104199004041234', 1),
('Eve', 'password222', 'eve@example.com', '12345678905', 35, 'F', '110105199005051234', 3),
('Frank', 'password333', 'frank@example.com', '12345678906', 40, 'M', '110106199006061234', 2),
('Grace', 'password444', 'grace@example.com', '12345678907', 26, 'F', '110107199007071234', 1),
('Henry', 'password555', 'henry@example.com', '12345678908', 33, 'M', '110108199008081234', 0),
('Ivy', 'password666', 'ivy@example.com', '12345678909', 29, 'F', '110109199009091234', 3),
('Jack', 'password777', 'jack@example.com', '12345678910', 38, 'M', '110110199010101234', 2),
('Kathy', 'password888', 'kathy@example.com', '12345678911', 23, 'F', '110111199011111234', 1),
('Leo', 'password999', 'leo@example.com', '12345678912', 31, 'M', '110112199012121234', 0),
('Mona', 'password000', 'mona@example.com', '12345678913', 27, 'F', '110113199013131234', 2),
('Nathan', 'password101', 'nathan@example.com', '12345678914', 34, 'M', '110114199014141234', 1),
('Olivia', 'password202', 'olivia@example.com', '12345678915', 32, 'F', '110115199015151234', 3),
('Peter', 'password303', 'peter@example.com', '12345678916', 41, 'M', '110116199016161234', 0),
('Quinn', 'password404', 'quinn@example.com', '12345678917', 36, 'F', '110117199017171234', 2),
('Rachel', 'password505', 'rachel@example.com', '12345678918', 24, 'F', '110118199018181234', 1),
('Steve', 'password606', 'steve@example.com', '12345678919', 39, 'M', '110119199019191234', 3);

INSERT INTO `flights` (`departure`, `departure_time`, `destination`, `arrival_time`, `flight_number`, `first_class_seats`, `business_class_seats`, `economy_class_seats`, `first_class_price`, `business_class_price`, `economy_class_price`, `departure_airport`, `arrival_airport`, `boarding_gate`, `aircraft_model`, `airline_company`, `has_meal`, `luggage_size_limit`, `is_international`) VALUES
('Beijing', '2024-12-01 08:00:00', 'Shanghai', '2024-12-01 10:30:00', 'CA101', 10, 20, 150, 5000, 3500, 1500, 'Beijing Capital International Airport', 'Shanghai Pudong International Airport', 'A1', 'Boeing 737', 'Air China', 1, 30, 0),
('Beijing', '2024-12-01 12:00:00', 'Shanghai', '2024-12-01 14:30:00', 'CA102', 15, 25, 160, 5200, 3600, 1550, 'Beijing Capital International Airport', 'Shanghai Pudong International Airport', 'A2', 'Airbus A320', 'Air China', 0, 35, 0),
('Shanghai', '2024-12-02 06:30:00', 'Guangzhou', '2024-12-02 09:00:00', 'MU201', 20, 30, 180, 4800, 3300, 1400, 'Shanghai Pudong International Airport', 'Guangzhou Baiyun International Airport', 'B1', 'Boeing 737', 'China Eastern Airlines', 1, 40, 0),
('Guangzhou', '2024-12-02 15:00:00', 'Beijing', '2024-12-02 17:30:00', 'MU202', 18, 22, 170, 4900, 3400, 1450, 'Guangzhou Baiyun International Airport', 'Beijing Capital International Airport', 'B2', 'Airbus A330', 'China Eastern Airlines', 0, 30, 0),
('Shanghai', '2024-12-03 09:00:00', 'Hong Kong', '2024-12-03 11:30:00', 'CX301', 8, 15, 140, 5500, 3800, 1600, 'Shanghai Pudong International Airport', 'Hong Kong International Airport', 'C1', 'Boeing 777', 'Cathay Pacific', 1, 25, 1),
('Hong Kong', '2024-12-03 13:30:00', 'Shanghai', '2024-12-03 16:00:00', 'CX302', 10, 18, 150, 5600, 3900, 1650, 'Hong Kong International Airport', 'Shanghai Pudong International Airport', 'C2', 'Airbus A350', 'Cathay Pacific', 0, 30, 1),
('Beijing', '2024-12-04 07:30:00', 'Los Angeles', '2024-12-04 11:30:00', 'AA501', 5, 10, 80, 20000, 15000, 7000, 'Beijing Capital International Airport', 'Los Angeles International Airport', 'D1', 'Boeing 777', 'American Airlines', 1, 50, 1),
('Los Angeles', '2024-12-04 16:00:00', 'Beijing', '2024-12-05 18:30:00', 'AA502', 6, 12, 90, 21000, 15500, 7100, 'Los Angeles International Airport', 'Beijing Capital International Airport', 'D2', 'Airbus A380', 'American Airlines', 0, 55, 1),
('Guangzhou', '2024-12-05 09:00:00', 'Tokyo', '2024-12-05 12:00:00', 'JL601', 10, 20, 150, 8000, 5500, 2200, 'Guangzhou Baiyun International Airport', 'Tokyo Narita International Airport', 'E1', 'Boeing 787', 'Japan Airlines', 1, 40, 1),
('Tokyo', '2024-12-05 14:30:00', 'Guangzhou', '2024-12-05 17:30:00', 'JL602', 12, 22, 160, 8100, 5600, 2250, 'Tokyo Narita International Airport', 'Guangzhou Baiyun International Airport', 'E2', 'Airbus A350', 'Japan Airlines', 0, 45, 1),
('Shanghai', '2024-12-06 10:00:00', 'Singapore', '2024-12-06 14:00:00', 'SQ901', 12, 20, 180, 9500, 6200, 2700, 'Shanghai Pudong International Airport', 'Singapore Changi Airport', 'F1', 'Boeing 747', 'Singapore Airlines', 1, 50, 1),
('Singapore', '2024-12-06 16:30:00', 'Shanghai', '2024-12-06 20:00:00', 'SQ902', 15, 25, 190, 9600, 6300, 2750, 'Singapore Changi Airport', 'Shanghai Pudong International Airport', 'F2', 'Airbus A350', 'Singapore Airlines', 0, 55, 1),
('Beijing', '2024-12-07 08:30:00', 'Seoul', '2024-12-07 11:00:00', 'OZ701', 10, 18, 140, 6500, 4200, 1800, 'Beijing Capital International Airport', 'Incheon International Airport', 'G1', 'Boeing 787', 'Asiana Airlines', 1, 35, 1),
('Seoul', '2024-12-07 13:00:00', 'Beijing', '2024-12-07 15:30:00', 'OZ702', 12, 20, 150, 6600, 4300, 1850, 'Incheon International Airport', 'Beijing Capital International Airport', 'G2', 'Airbus A330', 'Asiana Airlines', 0, 40, 1),
('Shenzhen', '2024-12-08 11:00:00', 'Taipei', '2024-12-08 13:30:00', 'CI501', 8, 15, 120, 7000, 4500, 2000, 'Shenzhen Baoan International Airport', 'Taiwan Taoyuan International Airport', 'H1', 'Boeing 737', 'China Airlines', 1, 25, 1),
('Taipei', '2024-12-08 15:00:00', 'Shenzhen', '2024-12-08 17:30:00', 'CI502', 10, 18, 130, 7100, 4600, 2050, 'Taiwan Taoyuan International Airport', 'Shenzhen Baoan International Airport', 'H2', 'Airbus A320', 'China Airlines', 0, 30, 1),
('Shanghai', '2024-12-09 07:00:00', 'Kuala Lumpur', '2024-12-09 10:00:00', 'MH301', 7, 15, 130, 8500, 5600, 2300, 'Shanghai Pudong International Airport', 'Kuala Lumpur International Airport', 'J1', 'Boeing 737', 'Malaysia Airlines', 1, 40, 1),
('Kuala Lumpur', '2024-12-09 12:30:00', 'Shanghai', '2024-12-09 16:00:00', 'MH302', 9, 18, 140, 8600, 5700, 2350, 'Kuala Lumpur International Airport', 'Shanghai Pudong International Airport', 'J2', 'Airbus A330', 'Malaysia Airlines', 0, 45, 1),
('Beijing', '2024-12-10 09:00:00', 'Bangkok', '2024-12-10 12:30:00', 'TG601', 6, 12, 100, 7300, 4800, 2000, 'Beijing Capital International Airport', 'Suvarnabhumi Airport', 'K1', 'Boeing 777', 'Thai Airways', 1, 30, 1),
('Bangkok', '2024-12-10 14:00:00', 'Beijing', '2024-12-10 17:30:00', 'TG602', 8, 15, 120, 7400, 4900, 2050, 'Suvarnabhumi Airport', 'Beijing Capital International Airport', 'K2', 'Airbus A380', 'Thai Airways', 0, 35, 1);



INSERT INTO `orders` (`user_id`, `flight_id`, `seat_type`, `price`, `order_status`, `payment_time`, `refund_time`) VALUES
(1, 5, '头等舱', 1200, '已支付', '2024-11-20 10:15:00', NULL),
(2, 10, '经济舱', 400, '已支付', '2024-11-21 14:30:00', NULL),
(3, 3, '商务舱', 800, '已支付', '2024-11-21 08:45:00', NULL),
(4, 15, '经济舱', 450, '已支付', '2024-11-20 11:00:00', NULL),
(5, 8, '头等舱', 1300, '已支付', '2024-11-22 16:15:00', NULL),
(6, 6, '商务舱', 750, '已支付', '2024-11-23 09:45:00', NULL),
(7, 20, '经济舱', 500, '已支付', '2024-11-23 13:00:00', NULL),
(1, 2, '头等舱', 1250, '已支付', '2024-11-24 18:00:00', NULL),
(2, 12, '商务舱', 700, '已支付', '2024-11-24 19:15:00', NULL),
(3, 9, '经济舱', 350, '已支付', '2024-11-25 07:30:00', NULL),
(4, 14, '头等舱', 1400, '已取消', '2024-11-20 12:00:00', '2024-11-20 14:00:00'),
(5, 7, '经济舱', 450, '已支付', '2024-11-21 16:45:00', NULL),
(6, 18, '商务舱', 820, '已支付', '2024-11-22 15:00:00', NULL),
(7, 1, '经济舱', 300, '已支付', '2024-11-23 09:30:00', NULL),
(1, 11, '头等舱', 1500, '未支付', NULL, NULL),
(2, 4, '经济舱', 400, '已支付', '2024-11-24 10:45:00', NULL),
(3, 13, '商务舱', 870, '已支付', '2024-11-25 14:30:00', NULL),
(4, 19, '头等舱', 1350, '已支付', '2024-11-21 08:15:00', NULL),
(5, 16, '经济舱', 390, '已支付', '2024-11-22 10:00:00', NULL),
(6, 5, '商务舱', 700, '已支付', '2024-11-22 20:15:00', NULL),
(7, 17, '经济舱', 320, '已支付', '2024-11-23 13:00:00', NULL),
(1, 3, '头等舱', 1200, '已支付', '2024-11-24 18:45:00', NULL),
(2, 8, '经济舱', 380, '已取消', '2024-11-25 07:30:00', '2024-11-25 09:00:00'),
(3, 10, '商务舱', 750, '已支付', '2024-11-20 15:00:00', NULL),
(4, 6, '经济舱', 400, '已支付', '2024-11-21 10:30:00', NULL),
(5, 14, '头等舱', 1400, '已支付', '2024-11-22 17:15:00', NULL),
(6, 2, '商务舱', 680, '已支付', '2024-11-23 12:00:00', NULL),
(7, 13, '经济舱', 350, '已支付', '2024-11-24 19:30:00', NULL),
(1, 20, '头等舱', 1450, '未支付', NULL, NULL),
(2, 9, '经济舱', 400, '已支付', '2024-11-21 08:30:00', NULL),
(3, 11, '商务舱', 800, '已支付', '2024-11-22 11:45:00', NULL),
(4, 16, '经济舱', 430, '已支付', '2024-11-23 09:15:00', NULL),
(5, 7, '头等舱', 1250, '已支付', '2024-11-24 14:30:00', NULL),
(6, 1, '商务舱', 750, '已支付', '2024-11-25 13:00:00', NULL),
(7, 18, '经济舱', 500, '已取消', '2024-11-20 17:30:00', '2024-11-20 19:00:00'),
(1, 15, '头等舱', 1500, '已支付', '2024-11-21 10:00:00', NULL),
(2, 4, '经济舱', 360, '已支付', '2024-11-22 09:45:00', NULL),
(3, 19, '商务舱', 810, '已支付', '2024-11-23 15:00:00', NULL),
(4, 12, '经济舱', 420, '已支付', '2024-11-24 11:00:00', NULL),
(5, 5, '头等舱', 1400, '未支付', NULL, NULL),
(6, 17, '商务舱', 720, '已支付', '2024-11-20 10:30:00', NULL),
(7, 3, '经济舱', 370, '已支付', '2024-11-21 08:00:00', NULL),
(1, 8, '头等舱', 1350, '已支付', '2024-11-22 16:45:00', NULL),
(2, 6, '商务舱', 770, '已支付', '2024-11-23 12:15:00', NULL),
(3, 10, '经济舱', 400, '已支付', '2024-11-24 15:00:00', NULL);
