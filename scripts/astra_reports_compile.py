import os
import csv

# Create or open the CSV file
csv_file_path = "reports_summary.csv"
with open(csv_file_path, mode='w', newline='') as csv_file:
    csv_writer = csv.writer(csv_file)
    csv_writer.writerow(["Design", "Number", "Area", "Total Power (mW)"])

    # Iterate through subdirectories
    for subdir in os.listdir():
        if os.path.isdir(subdir):
            design = subdir
            area_dir = os.path.join(subdir, "area")
            power_dir = os.path.join(subdir, "power")
            
            # Iterate through report files in the area subdirectory
            for rpt_file in os.listdir(area_dir):
                if rpt_file.endswith(".rpt"):
                    number = rpt_file.split("_")[2]
                    area_file_path = os.path.join(area_dir, rpt_file)
                    
                    area = 0
                    
                    with open(area_file_path, 'r') as area_report:
                        lines = area_report.readlines()
                        for row in lines: 
                            if "Number of cells:" in row:
                                area = " ".join(row.split())[-1]
                    
                    csv_writer.writerow([design, number, area, ""])  # Placeholder for total power

            # Iterate through report files in the power subdirectory
            for rpt_file in os.listdir(power_dir):
                if rpt_file.endswith("_power.rpt"):
                    number = rpt_file.split("_")[2]
                    power_file_path = os.path.join(power_dir, rpt_file)
                    
                    with open(power_file_path, 'r') as power_report:
                        lines = power_report.readlines()
                        total_power = lines[-2].split()[4]
                    
                    # Find corresponding area report
                    area_file_path = os.path.join(area_dir, f"{design}_{number}_area.rpt")
                    with open(area_file_path, 'r') as area_report:
                        area = area_report.readline().split()[-1]
                        
                    csv_writer.writerow([design, number, area, total_power])

print(f"Data has been written to {csv_file_path}")

