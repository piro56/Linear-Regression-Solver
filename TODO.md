BUG
- Test and fix problems when reopening new datasets (usually segfaults and not freeing memory correctly)
- Retry using char vector and casting to bool* for checklist (avoid using heap directly)

