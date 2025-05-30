import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
import seaborn as sns
from scipy.stats import chi2_contingency, spearmanr, ttest_ind
from scipy.stats import pearsonr

# Use publication-ready style
# plt.style.use('seaborn-paper')
plt.rcParams['font.family'] = 'serif'
plt.rcParams['font.size'] = 10
plt.rcParams['axes.labelsize'] = 12
plt.rcParams['axes.titlesize'] = 14

# Color-blind friendly palette
colors = sns.color_palette("colorblind")

# Reload to ensure fresh state
df = pd.read_csv("statistic.csv")
for col in ["Bug", "Crpt", "Failed"]:
    df[col] = df[col].astype(str).str.upper() == "TRUE"

# ------------------------------------------------------------------------------
# Helper: classify each row into a single high‑level outcome
def classify(row):
    if row["Failed"]:
        return "Failed (compile/KLEE)"
    if row["Crpt"]:
        return "Memory corruption"
    if row["Bug"]:
        return "Logical bug"
    return "Clean"

df["Outcome"] = df.apply(classify, axis=1)

# ------------------------------------------------------------------------------
# 1)  Pivot table: bug‑rate by <Problem, LLM, Tag>
pivot_problem = (
    df.groupby(["Problem", "LLM", "Tag"])
        .agg(total=("Bug", "size"), bugs=("Bug", "sum"))
        .assign(bug_rate=lambda x: (x["bugs"] / x["total"]).round(3))
        .reset_index()
)

# ------------------------------------------------------------------------------
# Choose a single palette for all bar plots
bar_palette = plt.get_cmap("tab20").colors

# 2) Stacked bar: outcome distribution per LLM
outcome_counts = (
    df.groupby(["LLM", "Outcome"])
      .size()
      .unstack(fill_value=0)
)
labels = outcome_counts.index
outcomes = outcome_counts.columns
bottom = np.zeros(len(labels))

fig, ax = plt.subplots(figsize=(7,4))
for idx, outcome in enumerate(outcomes):
    vals = outcome_counts[outcome].values
    ax.bar(labels, vals, bottom=bottom, label=outcome, color=bar_palette[idx % len(bar_palette)])
    bottom += vals


ax.set_ylabel("Number of samples")
ax.set_title("Distribution of Code Quality Outcomes\nAcross Different LLM Models")
ax.set_xlabel('Language Model')
ax.set_ylabel('Number of Generated Samples')
ax.legend(title="Outcome Type", bbox_to_anchor=(1.05, 1), loc='upper left')
plt.tight_layout()
plt.savefig('figure2_outcome_distribution.png', bbox_inches='tight', dpi=300)
#plt.show()

# ------------------------------------------------------------------------------
# 3) Grouped bar: bug‑rate per problem template by LLM
# Calculate bug rate per problem template by LLM
bug_prob_llm = (
    df.groupby(["Problem", "LLM"])["Bug"]
      .mean()
      .unstack(fill_value=0) * 100
)
problems = bug_prob_llm.index
x = np.arange(len(problems))
width = 0.2
fig, ax = plt.subplots(figsize=(10,4))
for idx, llm in enumerate(bug_prob_llm.columns):
    ax.bar(x + idx*width - width, bug_prob_llm[llm], width=width, label=llm.capitalize(), color=bar_palette[idx % len(bar_palette)])

ax.set_xticks(x)
ax.set_xticklabels(problems, rotation=45, ha="right")
ax.set_ylabel("Bug rate (%)")
ax.set_title("Bug Rates Across Problem Templates and LLM Models")
ax.set_xlabel('Problem Template')
ax.legend()
plt.tight_layout()
plt.savefig('figure3_problem_llm_comparison.png', bbox_inches='tight', dpi=300)
#plt.show()

# ------------------------------------------------------------------------------
# 4) Grouped bar: bug‑rate per problem template by Prompt Tag
# Calculate bug rate per problem template by Prompt Tag
bug_prob_tag = (
    df.groupby(["Problem", "Tag"])["Bug"]
      .mean()
      .unstack(fill_value=0) * 100
)
tags = bug_prob_tag.columns
fig, ax = plt.subplots(figsize=(10,4))
for idx, tag in enumerate(tags):
    ax.bar(x + idx*width - width, bug_prob_tag[tag], width=width, label=tag, color=bar_palette[idx % len(bar_palette)])

ax.set_xticks(x)
ax.set_xticklabels(problems, rotation=45, ha="right")
ax.set_ylabel("Bug rate (%)")
ax.set_title("Impact of Prompt Engineering on Bug Rates\nAcross Different Problem Templates")
ax.set_xlabel('Problem Template')
ax.legend()
plt.tight_layout()
plt.savefig('figure4_prompt_impact.png', bbox_inches='tight', dpi=300)
#plt.show()

# Add confidence intervals to bug rates
confidence = 0.95
def calculate_ci(data):
    ci = stats.t.interval(confidence, len(data)-1, loc=np.mean(data), scale=stats.sem(data))
    return pd.Series({'mean': np.mean(data), 'ci_lower': ci[0], 'ci_upper': ci[1]})

# Calculate bug rates with confidence intervals
bug_rates_with_ci = (
    df.groupby(['LLM'])['Bug']
    .apply(calculate_ci)
    .unstack()
)

# Create bar plot with error bars
fig, ax = plt.subplots(figsize=(10, 6))
x = np.arange(len(bug_rates_with_ci.index))
ax.bar(x, bug_rates_with_ci['mean'] * 100, 
       yerr=[(bug_rates_with_ci['mean'] - bug_rates_with_ci['ci_lower']) * 100,
             (bug_rates_with_ci['ci_upper'] - bug_rates_with_ci['mean']) * 100],
       capsize=5, color=colors, alpha=0.8)

ax.set_title('Statistical Analysis of Bug Rates by LLM\nwith 95% Confidence Intervals', pad=20)
ax.set_xlabel('Language Model', labelpad=10)
ax.set_ylabel('Bug Rate (%)', labelpad=10)
ax.set_xticks(x)
ax.set_xticklabels(bug_rates_with_ci.index)
plt.tight_layout()
plt.savefig('figure5_statistical_analysis.png', bbox_inches='tight', dpi=300)
#plt.show()

# ------------------------------------------------------------------------------
# Create multiple heatmaps for different metrics
fig = plt.figure(figsize=(16, 12))
gs = fig.add_gridspec(2, 3, width_ratios=[1, 1, 0.1])
ax1 = fig.add_subplot(gs[0, 0])
ax2 = fig.add_subplot(gs[0, 1])
ax3 = fig.add_subplot(gs[1, 0])
ax4 = fig.add_subplot(gs[1, 1])
cax = fig.add_subplot(gs[:, 2])  # Shared colorbar axis

# Handle empty tags by replacing them with "no_tag"
df['Tag'] = df['Tag'].fillna('no_injection')

# Calculate all heatmaps first to determine global min/max
bug_heat = df.groupby(["LLM", "Tag"])["Bug"].mean().unstack(fill_value=0) * 100
crpt_heat = df.groupby(["LLM", "Tag"])["Crpt"].mean().unstack(fill_value=0) * 100
fail_heat = df.groupby(["LLM", "Tag"])["Failed"].mean().unstack(fill_value=0) * 100
df['Any_Error'] = df['Bug'] | df['Crpt'] | df['Failed']
total_heat = df.groupby(["LLM", "Tag"])["Any_Error"].mean().unstack(fill_value=0) * 100

# Reorder columns to put 'no_injection' first
column_order = ['no_injection'] + [col for col in bug_heat.columns if col != 'no_injection']
bug_heat = bug_heat[column_order]
crpt_heat = crpt_heat[column_order]
fail_heat = fail_heat[column_order]
total_heat = total_heat[column_order]

# Find global min/max for consistent scale
vmin = min(
    bug_heat.min().min(), 
    crpt_heat.min().min(), 
    fail_heat.min().min(), 
    total_heat.min().min()
)
vmax = max(
    bug_heat.max().max(), 
    crpt_heat.max().max(), 
    fail_heat.max().max(), 
    total_heat.max().max()
)

# Modify annotation format to show only percentage
def annot_fmt(val):
    return f'{val:.1f}%'

# Create annotation matrices
bug_annot = bug_heat.applymap(annot_fmt)
crpt_annot = crpt_heat.applymap(annot_fmt)
fail_annot = fail_heat.applymap(annot_fmt)
total_annot = total_heat.applymap(annot_fmt)

# Common plotting parameters
common_kwargs = {
    'cmap': 'YlOrRd',
    'vmin': vmin,
    'vmax': vmax,
    'fmt': '',
    'cbar': False
}

# Plot heatmaps
sns.heatmap(bug_heat, ax=ax1, annot=bug_annot, **common_kwargs)
sns.heatmap(crpt_heat, ax=ax2, annot=crpt_annot, **common_kwargs)
sns.heatmap(fail_heat, ax=ax3, annot=fail_annot, **common_kwargs)
sns.heatmap(total_heat, ax=ax4, annot=total_annot, **common_kwargs)

# Add titles
ax1.set_title('(a) Logical Bug Rate', pad=10)
ax2.set_title('(b) Memory Corruption Rate', pad=10)
ax3.set_title('(c) Compilation/KLEE Failure Rate', pad=10)
ax4.set_title('(d) Overall Error Rate', pad=10)

ax1.set_xlabel(str())
ax2.set_xlabel(str())
ax3.set_xlabel(str())
ax4.set_xlabel(str())

# Add shared colorbar (make it even thinner)
norm = plt.Normalize(vmin=vmin, vmax=vmax)
sm = plt.cm.ScalarMappable(cmap='YlOrRd', norm=norm)
cbar = fig.colorbar(sm, cax=cax, fraction=0.01, pad=0.04)  # fraction reduced for thinner bar
cbar.set_label('Rate (%)', labelpad=10)

# Adjust layout and add main title
plt.suptitle('Error Rates Across LLMs and Prompt Types', 
             fontsize=16, y=1.02)
plt.tight_layout()
plt.savefig('figure1_error_analysis_heatmaps.png', 
            bbox_inches='tight', dpi=300)
plt.show()

# Print summary statistics
print("\nSummary Statistics:")
print("-" * 80)
print(f"Total samples analyzed: {len(df)}")
print(f"Overall bug rate: {df['Bug'].mean()*100:.1f}% ± {stats.sem(df['Bug'])*100:.1f}%")
print(f"Memory corruption rate: {df['Crpt'].mean()*100:.1f}% ± {stats.sem(df['Crpt'])*100:.1f}%")
print("-" * 80)

# Improve bar plots with common styling
def style_plot(ax, title, xlabel, ylabel):
    ax.grid(True, axis='y', linestyle='--', alpha=0.7)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.set_title(title, pad=20)
    ax.set_xlabel(xlabel, labelpad=10)
    ax.set_ylabel(ylabel, labelpad=10)

# Apply to existing plots...
# For outcome distribution plot:
style_plot(ax, 
    "Distribution of Code Quality Outcomes\nAcross Different LLM Models",
    "Language Model",
    "Number of Generated Samples")
ax.legend(title="Outcome Type", bbox_to_anchor=(1.05, 1), loc='upper left')

# ------------------------------------------------------------------------------
# Add comprehensive statistical analysis at the end
print("\nComprehensive Statistical Analysis")
print("=" * 80)

# 1. Overall Statistics
print("\n1. Overall Statistics:")
print("-" * 40)
total_samples = len(df)
print(f"Total Samples: {total_samples}")
for metric in ['Bug', 'Crpt', 'Failed', 'Any_Error']:
    mean = df[metric].mean() * 100
    std = df[metric].std() * 100
    ci = stats.t.interval(0.95, len(df)-1, loc=mean, scale=stats.sem(df[metric])*100)
    print(f"{metric.replace('_', ' ')} Rate: {mean:.1f}% ± {std:.1f}% (95% CI: [{ci[0]:.1f}%, {ci[1]:.1f}%])")

# 2. LLM Comparisons
print("\n2. LLM Performance Comparison:")
print("-" * 40)
for llm in df['LLM'].unique():
    llm_data = df[df['LLM'] == llm]
    print(f"\n{llm.upper()}:")
    for metric in ['Bug', 'Crpt', 'Failed']:
        mean = llm_data[metric].mean() * 100
        std = llm_data[metric].std() * 100
        print(f"  {metric} Rate: {mean:.1f}% ± {std:.1f}%")

# 3. Statistical Tests
print("\n3. Statistical Tests:")
print("-" * 40)
# Chi-square test for independence between LLM and bug occurrence
contingency = pd.crosstab(df['LLM'], df['Bug'])
chi2, p_val, _, _ = chi2_contingency(contingency)
print(f"Chi-square test (LLM × Bug): χ²={chi2:.2f}, p={p_val:.4f}")

# 4. Correlation Analysis
print("\n4. Correlation Analysis:")
print("-" * 40)
# Correlation between different error types
corr_matrix = df[['Bug', 'Crpt', 'Failed']].corr()
print("\nError Type Correlations:")
print(corr_matrix.round(3))

# 5. Prompt Tag Analysis
print("\n5. Prompt Tag Effect Analysis:")
print("-" * 40)
tag_stats = df.groupby('Tag').agg({
    'Bug': ['mean', 'std', 'count'],
    'Crpt': ['mean', 'std'],
    'Failed': ['mean', 'std']
}).round(3)
print("\nPrompt Tag Statistics:")
print(tag_stats)

# 6. Problem Template Analysis
print("\n6. Problem Template Analysis:")
print("-" * 40)
problem_stats = df.groupby('Problem')['Bug'].agg(['mean', 'std', 'count']).round(3)
print("\nProblem Template Statistics:")
print(problem_stats)

print("\n7. Key Findings:")
print("-" * 40)
# Calculate and print most problematic combinations
worst_combos = df.groupby(['LLM', 'Tag', 'Problem'])['Bug'].mean().sort_values(ascending=False).head()
print("\nMost Error-Prone Combinations (LLM × Tag × Problem):")
print(worst_combos)

print("=" * 80)

print("...done")

def create_combined_bar_plots(df):
    fig = plt.figure(figsize=(15, 12))
    gs = fig.add_gridspec(2, 2, hspace=0.3)
    
    # 1. Stacked bar (top left)
    ax1 = fig.add_subplot(gs[0, 0])
    outcome_counts = df.groupby(["LLM", "Outcome"]).size().unstack(fill_value=0)
    bottom = np.zeros(len(outcome_counts.index))
    for idx, outcome in enumerate(outcome_counts.columns):
        ax1.bar(outcome_counts.index, outcome_counts[outcome], bottom=bottom, 
                label=outcome, color=bar_palette[idx % len(bar_palette)])
        bottom += outcome_counts[outcome]
    ax1.set_title('(a) Outcome Distribution by LLM')
    ax1.set_xlabel('Language Model')