/*
 Navicat Premium Data Transfer

 Source Server         : MySQL
 Source Server Type    : MySQL
 Source Server Version : 100428 (10.4.28-MariaDB)
 Source Host           : localhost:3306
 Source Schema         : gaboot

 Target Server Type    : MySQL
 Target Server Version : 100428 (10.4.28-MariaDB)
 File Encoding         : 65001

 Date: 22/10/2023 23:16:47
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for carts
-- ----------------------------
DROP TABLE IF EXISTS `carts`;
CREATE TABLE `carts`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `customerId` int NULL DEFAULT NULL,
  `productId` int NULL DEFAULT NULL,
  `price` int NULL DEFAULT NULL,
  `quantity` int NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `customerId`(`customerId` ASC) USING BTREE,
  INDEX `productId`(`productId` ASC) USING BTREE,
  CONSTRAINT `carts_ibfk_1` FOREIGN KEY (`customerId`) REFERENCES `master_customers` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `carts_ibfk_2` FOREIGN KEY (`productId`) REFERENCES `master_products` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of carts
-- ----------------------------
INSERT INTO `carts` VALUES (2, 1, 1, 12000, 12, '2023-10-20 14:43:12', '2023-10-20 14:43:12');

-- ----------------------------
-- Table structure for categories
-- ----------------------------
DROP TABLE IF EXISTS `categories`;
CREATE TABLE `categories`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `description` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `imagePath` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `thumbnailPath` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `name`(`name` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 14 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of categories
-- ----------------------------
INSERT INTO `categories` VALUES (9, 'motherboard', 'Motherboard AMD', 'asu.png', 'asu_tbl.png', '2023-10-20 09:58:42', '2023-10-20 09:58:42');
INSERT INTO `categories` VALUES (11, 'GPU', 'GPU AMD', 'asu.png', 'asu_tbl.png', '2023-10-20 09:59:41', '2023-10-20 09:59:41');
INSERT INTO `categories` VALUES (12, 'CPU', 'CPU AMD', 'storage/product/pictures/undefined1697809701567.jpg', 'storage/product/pictures/thumb/undefined1697809701567_thumb.jpg', '2023-10-20 10:00:06', '2023-10-20 13:48:21');
INSERT INTO `categories` VALUES (13, 'GPU NVIDIA', 'NVIDIA BAGUS', 'storage/category/pictures/GPU_NVIDIA1697809677915.jpg', 'storage/category/pictures/thumb/GPU_NVIDIA1697809677915_thumb.jpg', '2023-10-20 13:47:57', '2023-10-20 13:47:57');

-- ----------------------------
-- Table structure for master_customers
-- ----------------------------
DROP TABLE IF EXISTS `master_customers`;
CREATE TABLE `master_customers`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `firstname` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `lastname` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `username` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `email` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `phoneNumber` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `addressDetail` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `latitude` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `longitude` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `password` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `token` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `isActive` tinyint(1) NULL DEFAULT NULL,
  `imagePath` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `thumbnailPath` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `username`(`username` ASC) USING BTREE,
  UNIQUE INDEX `email`(`email` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 61 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of master_customers
-- ----------------------------
INSERT INTO `master_customers` VALUES (1, 'Ale', 'Baba', 'alebaba', 'ali@mail.com', '0812121212', 'Jl. Konoha', '19219219.1121.', '12121.212121', '$2b$10$WbqPalXlQVBHul3GE1gRluNZ24600yTXdMyWoDCZ4TiFI5dk0hi6S', NULL, 1, '325497847_508448074706890_6310011937187920434_n.jpg', '325497847_508448074706890_6310011937187920434_n.jpg', '0000-00-00 00:00:00', '2023-10-21 09:30:06');
INSERT INTO `master_customers` VALUES (60, 'Elaina', 'Magica', 'elaina023', 'elaina@mail.com', '', 'Jl. Konoha', '', '', '$2b$10$LNgVYn.uQluagGqZSsN.3uwwFmDRIdBj8OQ78HhrKUeTORvDM5tqq', NULL, 1, 'uploads\\customers\\pictures\\elaina023.jpg', 'uploads\\customers\\pictures\\thumbnail\\elaina023.jpg', '2023-10-22 18:14:56', '2023-10-22 18:14:56');

-- ----------------------------
-- Table structure for master_menus
-- ----------------------------
DROP TABLE IF EXISTS `master_menus`;
CREATE TABLE `master_menus`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `menuName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `menuIcon` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `backendUrl` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `frontendUrl` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `menuHaveChild` tinyint(1) NULL DEFAULT NULL,
  `menuIsActive` tinyint(1) NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of master_menus
-- ----------------------------

-- ----------------------------
-- Table structure for master_products
-- ----------------------------
DROP TABLE IF EXISTS `master_products`;
CREATE TABLE `master_products`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `description` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `stock` int NULL DEFAULT NULL,
  `size` int NULL DEFAULT NULL,
  `category_id` int NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `category_id`(`category_id` ASC) USING BTREE,
  CONSTRAINT `master_products_ibfk_1` FOREIGN KEY (`category_id`) REFERENCES `categories` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of master_products
-- ----------------------------
INSERT INTO `master_products` VALUES (1, 'AMD Ryzen 5 5600x', '6 Core 12 Thread', 12, NULL, 12, '2023-10-20 10:12:19', '2023-10-20 10:12:19');

-- ----------------------------
-- Table structure for master_roles
-- ----------------------------
DROP TABLE IF EXISTS `master_roles`;
CREATE TABLE `master_roles`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `roleName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of master_roles
-- ----------------------------

-- ----------------------------
-- Table structure for master_submenus
-- ----------------------------
DROP TABLE IF EXISTS `master_submenus`;
CREATE TABLE `master_submenus`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `menuId` int NULL DEFAULT NULL,
  `submenuName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `submenuIcon` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `frontendUrl` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `backendUrl` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `submenuIsActive` tinyint(1) NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `menuId`(`menuId` ASC) USING BTREE,
  CONSTRAINT `master_submenus_ibfk_1` FOREIGN KEY (`menuId`) REFERENCES `master_menus` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of master_submenus
-- ----------------------------

-- ----------------------------
-- Table structure for master_users
-- ----------------------------
DROP TABLE IF EXISTS `master_users`;
CREATE TABLE `master_users`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `userName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `email` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `firstName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `lastName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `password` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `token` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `isActive` tinyint(1) NULL DEFAULT 1,
  `imgPath` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `imgThumbPath` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `roleId` int NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `userName`(`userName` ASC) USING BTREE,
  UNIQUE INDEX `email`(`email` ASC) USING BTREE,
  INDEX `roleId`(`roleId` ASC) USING BTREE,
  CONSTRAINT `master_users_ibfk_1` FOREIGN KEY (`roleId`) REFERENCES `master_roles` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of master_users
-- ----------------------------

-- ----------------------------
-- Table structure for role_accesses
-- ----------------------------
DROP TABLE IF EXISTS `role_accesses`;
CREATE TABLE `role_accesses`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `roleId` int NULL DEFAULT NULL,
  `menuId` int NULL DEFAULT NULL,
  `submenuId` int NULL DEFAULT NULL,
  `frontendUrl` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `backendUrl` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `create` tinyint(1) NULL DEFAULT NULL,
  `read` tinyint(1) NULL DEFAULT NULL,
  `updates` tinyint(1) NULL DEFAULT NULL,
  `delete` tinyint(1) NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `roleId`(`roleId` ASC) USING BTREE,
  INDEX `menuId`(`menuId` ASC) USING BTREE,
  INDEX `submenuId`(`submenuId` ASC) USING BTREE,
  CONSTRAINT `role_accesses_ibfk_1` FOREIGN KEY (`roleId`) REFERENCES `master_roles` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `role_accesses_ibfk_2` FOREIGN KEY (`menuId`) REFERENCES `master_menus` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `role_accesses_ibfk_3` FOREIGN KEY (`submenuId`) REFERENCES `master_submenus` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of role_accesses
-- ----------------------------

-- ----------------------------
-- Table structure for role_menus
-- ----------------------------
DROP TABLE IF EXISTS `role_menus`;
CREATE TABLE `role_menus`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `roleId` int NULL DEFAULT NULL,
  `menuId` int NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `roleId`(`roleId` ASC) USING BTREE,
  INDEX `menuId`(`menuId` ASC) USING BTREE,
  CONSTRAINT `role_menus_ibfk_1` FOREIGN KEY (`roleId`) REFERENCES `master_roles` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `role_menus_ibfk_2` FOREIGN KEY (`menuId`) REFERENCES `master_menus` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of role_menus
-- ----------------------------

-- ----------------------------
-- Table structure for role_submenus
-- ----------------------------
DROP TABLE IF EXISTS `role_submenus`;
CREATE TABLE `role_submenus`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `roleId` int NULL DEFAULT NULL,
  `submenuId` int NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  `roleMenuId` int NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `roleId`(`roleId` ASC) USING BTREE,
  INDEX `submenuId`(`submenuId` ASC) USING BTREE,
  INDEX `roleMenuId`(`roleMenuId` ASC) USING BTREE,
  CONSTRAINT `role_submenus_ibfk_1` FOREIGN KEY (`roleId`) REFERENCES `master_roles` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `role_submenus_ibfk_2` FOREIGN KEY (`submenuId`) REFERENCES `master_submenus` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `role_submenus_ibfk_3` FOREIGN KEY (`roleMenuId`) REFERENCES `role_menus` (`id`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of role_submenus
-- ----------------------------

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `userName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `email` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `firstName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `lastName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `password` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `token` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `isActive` tinyint(1) NULL DEFAULT 1,
  `imgPath` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `imgThumbPath` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `roleId` int NULL DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `userName`(`userName` ASC) USING BTREE,
  UNIQUE INDEX `email`(`email` ASC) USING BTREE,
  INDEX `roleId`(`roleId` ASC) USING BTREE,
  CONSTRAINT `users_ibfk_1` FOREIGN KEY (`roleId`) REFERENCES `master_roles` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of users
-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
