package more;

import java.util.ArrayList;
import java.util.List;

/**
 * 迪米特原则
 */
public class LodDesign {

    public static void main(String[] args) {
        LodDesign factoryDesign1 = new LodDesign();
        factoryDesign1.test1();
    }

    private void test1() {
        CompanyManager companyManager = new CompanyManager();
        companyManager.printAllEmployee(new SubCompanyManager());
    }

    // 总公司员工
    class Employee {
        private String id;

        public String getId() {
            return id;
        }

        public void setId(String id) {
            this.id = id;
        }
    }

    // 分公司员工
    class SubEmployee {
        private String id;

        public String getId() {
            return id;
        }

        public void setId(String id) {
            this.id = id;
        }
    }

    // 子公司管理
    class SubCompanyManager {
        public List<SubEmployee> getAllEmployee() {
            List<SubEmployee> list = new ArrayList<>();
            for (int i = 1; i < 5; i++) {
                SubEmployee subEmployee = new SubEmployee();
                // 给分公司人员顺序分配一个ID
                subEmployee.setId("分公司" + i);
                list.add(subEmployee);
            }
            return list;
        }
    }

    // 总公司管理
    class CompanyManager {
        public List<Employee> getAllEmployee() {
            List<Employee> list = new ArrayList<>();
            for (int i = 1; i < 3; i++) {
                Employee employee = new Employee();
                // 给总公司人员顺序分配一个ID
                employee.setId("总公司" + i);
                list.add(employee);
            }
            return list;
        }

        public void printAllEmployee(SubCompanyManager subCompanyManager) {
            // 分公司员工
            List<SubEmployee> subEmployeeList = subCompanyManager.getAllEmployee();
            for (SubEmployee subEmployee : subEmployeeList) {
                System.out.println(subEmployee.getId());
            }

            // 总公司员工
            List<Employee> employeeList = getAllEmployee();
            for (Employee employee : employeeList) {
                System.out.println(employee.getId());
            }
        }
    }

}
