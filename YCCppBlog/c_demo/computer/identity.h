//
// Created by 杨充 on 2025/7/15.
// 身份枚举和通用结构定义
//


// 状态枚举
typedef enum {
    PENDING,
    APPROVED,
    REJECTED,
    CANCELLED
} OrderStatus;

// 时间段枚举
typedef enum {
    MORNING,
    AFTERNOON
} TimeSlot;


// 机房信息结构体
typedef struct {
    int roomId;
    int capacity;
    char description[100];
} ComputerRoom;




// 预约记录结构体
typedef struct {
    int orderId;
    time_t requestTime;
    time_t reservationDate;
    int studentId;
    char studentName[50];
    int teacherId;
    char teacherName[50];
    int roomId;
    TimeSlot timeSlot;
    OrderStatus status;
    time_t processTime;
} ReservationOrder;

