import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# --- Configuration ---
CSV_FILE = 'results.csv'
OUTPUT_DIR = 'plots'
PLOT_FORMAT = 'png'

# --- Main plotting logic ---
def main():
    # Check if CSV file exists
    if not os.path.exists(CSV_FILE):
        print(f"Error: '{CSV_FILE}' not found. Please run the C++ program first.")
        return

    # Create output directory
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)
        print(f"Created directory: '{OUTPUT_DIR}'")

    # Load and process data
    df = pd.read_csv(CSV_FILE)
    
    # Calculate the mean of the runs for each (DataStructure, N) group
    avg_df = df.groupby(['DataStructure', 'N']).mean().reset_index()
    
    # Set plot style
    sns.set_theme(style="whitegrid")

    # --- Filter data for plots 1 and 2 ---
    # This creates a new dataframe that excludes the p=0.25 and p=0.75 Skip List variants
    plot1_2_df = avg_df[~avg_df['DataStructure'].isin(['SkipList_p025', 'SkipList_p075'])]

    # --- Plot 1: Insertion Time vs. N ---
    print("Generating Plot 1: Insertion Time...")
    plt.figure(figsize=(12, 8))
    plot1 = sns.lineplot(
        data=plot1_2_df,  # Use filtered data
        x='N',
        y='InsertionTime_us',
        hue='DataStructure',
        style='DataStructure',
        markers=True,
        dashes=True
    )
    plot1.set_title('Average Insertion Time vs. Number of Nodes (N)', fontsize=16)
    plot1.set_xlabel('Number of Nodes (N)', fontsize=12)
    plot1.set_ylabel('Average Insertion Time (microseconds)', fontsize=12)
    plot1.set_xscale('log', base=2)
    plot1.set_yscale('log')
    plot1.legend(title='Data Structure')
    plt.tight_layout()
    plt.savefig(os.path.join(OUTPUT_DIR, f'1_insertion_time.{PLOT_FORMAT}'))
    plt.close()

    # --- Plot 2: Search Time vs. N ---
    print("Generating Plot 2: Search Time...")
    plt.figure(figsize=(12, 8))
    plot2 = sns.lineplot(
        data=plot1_2_df,  # Use filtered data
        x='N',
        y='AvgSearchTime_ns',
        hue='DataStructure',
        style='DataStructure',
        markers=True,
        dashes=True
    )
    plot2.set_title('Average Search Time vs. Number of Nodes (N)', fontsize=16)
    plot2.set_xlabel('Number of Nodes (N)', fontsize=12)
    plot2.set_ylabel('100M Average Search Time (nanoseconds)', fontsize=12)
    plot2.set_xscale('log', base=2)
    # y-axis for search is often better linear to see small differences
    # plot2.set_yscale('log') 
    plot2.legend(title='Data Structure')
    plt.tight_layout()
    plt.savefig(os.path.join(OUTPUT_DIR, f'2_search_time.{PLOT_FORMAT}'))
    plt.close()

    # --- Plot 3: Height vs. N ---
    print("Generating Plot 3: Height...")
    plt.figure(figsize=(12, 8))
    plot3 = sns.lineplot(
        data=avg_df,  # Use original, unfiltered data for the height comparison
        x='N',
        y='Height',
        hue='DataStructure',
        style='DataStructure',
        markers=True,
        dashes=True
    )
    plot3.set_title('Average Height vs. Number of Nodes (N)', fontsize=16)
    plot3.set_xlabel('Number of Nodes (N)', fontsize=12)
    plot3.set_ylabel('Average Height', fontsize=12)
    plot3.set_xscale('log', base=2)
    plot3.set_yscale('log')
    plot3.legend(title='Data Structure')
    plt.tight_layout()
    plt.savefig(os.path.join(OUTPUT_DIR, f'3_height.{PLOT_FORMAT}'))
    plt.close()

    print(f"\nAll plots have been saved to the '{OUTPUT_DIR}' directory.")

if __name__ == '__main__':
    main()
