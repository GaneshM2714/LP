#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

class FIFO
{
    vector<int> pages;
    int frame = 3;
    int idx = 0;
    int pg_hit = 0, pg_miss = 0;
    vector<vector<int>> steps;

public:
    void takeinput()
    {
        cout << "Enter Reference String(Space Seperated,-1 to end): ";
        while (1)
        {
            int t;
            cin >> t;

            if (t == -1)
                break;
            pages.push_back(t);
        }
        processpages();
    }

    void processpages()
    {
        vector<int> frames(frame, -1);

        for (auto &pg : pages)
        {
            bool skip = false;
            for (auto &j : frames)
            {
                if (j == -1 || j == pg)
                {
                    if (j == pg)
                        pg_hit++;
                    else
                    {
                        pg_miss++;
                        j = pg;
                    }
                    skip = true;
                    break;
                }
            }

            if (skip)
            {
                steps.push_back(frames);
                continue;
            }

            frames[idx] = pg;
            pg_miss++;
            idx = (idx + 1) % frame;
            steps.push_back(frames);
        }

        displayresults();
    }

    void displayresults()
    {
        for (auto &r : steps)
        {
            for (auto &c : r)
            {
                cout << c << " ";
            }
            cout << "\n";
            sleep(1);
        }

        cout << "Page Hit: " << pg_hit << "\nPage Miss: " << pg_miss << "\n";
        cout << "\nPage hit ratio% : " << (float)(pg_hit * 100) / pages.size() << endl;
        cout << "\nPage miss ratio% : " << (float)(pg_miss * 100) / pages.size() << endl;
    }
};

class optimal
{
    int frame = 4;
    vector<int> pages;
    int idx = 0;
    int pg_hit = 0, pg_miss = 0;
    vector<vector<int>> steps;

public:
    void takeinput()
    {
        cout << "Enter Reference String(Space Seperated,-1 to end): ";
        while (1)
        {
            int t;
            cin >> t;

            if (t == -1)
                break;
            pages.push_back(t);
        }
        processpages();
    }

    int searchin(vector<int> &v, int j, int pg)
    {
        for (int i = j; i<v.size();i++)
        {
            if (pg == v[i])
                return i;
        }
        return -1;
    }
    void processpages()
    {
        vector<int> frames(frame, -1);

        for (int i = 0; i < pages.size(); i++)
        {
            int pg = pages[i];

            bool skip = false;
            for (auto &j : frames)
            {
                if (j == -1 || j == pg)
                {
                    if (j == pg)
                        pg_hit++;
                    else
                    {
                        pg_miss++;
                        j = pg;
                    }
                    skip = true;
                    break;
                }
            }

            if (skip)
            {
                steps.push_back(frames);
                continue;
            }

            int farthest = -1;

            for (int j = 0; j < frame; j++)
            {
                int curr_pg = frames[j];
                int x = searchin(pages, i + 1, curr_pg);
                if (x == -1)
                {
                    frames[j] = pg;
                    farthest = -1;
                    pg_miss++;
                    break;
                }
                else
                {
                    if (farthest < x)
                        farthest = x;
                }
            }

            if (farthest == -1)
                continue;
            int idx = searchin(frames, 0, pages[farthest]);
            frames[idx] = pg;
            pg_miss++;
            steps.push_back(frames);
        }
        displayresults();
    }

    void displayresults()
    {
        for (auto &r : steps)
        {
            for (auto &c : r)
            {
                cout << c << " ";
            }
            cout << "\n";
            sleep(1);
        }

        cout << "Page Hit: " << pg_hit << "\nPage Miss: " << pg_miss << "\n";
        cout << "\nPage hit ratio% : " << (float)(pg_hit * 100) / pages.size() << endl;
        cout << "\nPage miss ratio% : " << (float)(pg_miss * 100) / pages.size() << endl;
    }
};

class LRU
{
    int frame = 3;
    vector<int> pages;
    int idx = 0;
    int pg_hit = 0, pg_miss = 0;
    vector<vector<int>> steps;

public:
    void takeinput()
    {
        cout << "Enter Reference String(Space Seperated,-1 to end): ";
        while (1)
        {
            int t;
            cin >> t;

            if (t == -1)
                break;
            pages.push_back(t);
        }
        processpages();
    }

    int searchin(vector<int> &v, int j, int pg)
    {
        for (int i = j; i >=0; i--)
        {
            if (pg == v[i])
                return i;
        }
        return -1;
    }
    void processpages()
    {
        vector<int> frames(frame, -1);

        for (int i = 0; i < pages.size(); i++)
        {
            int pg = pages[i];

            bool skip = false;
            for (auto &j : frames)
            {
                if (j == -1 || j == pg)
                {
                    if (j == pg)
                        pg_hit++;
                    else
                    {
                        pg_miss++;
                        j = pg;
                    }
                    skip = true;
                    break;
                }
            }

            if (skip)
            {
                steps.push_back(frames);
                continue;
            }


            int farthest = INT_MAX;

            for (int j = 0; j < frame; j++)
            {
                int curr_pg = frames[j];
                int x = searchin(pages, i - 1, curr_pg);
                if (farthest > x)
                    farthest = x;
            }

            int idx = searchin(frames, frame, pages[farthest]);
            frames[idx] = pg;
            pg_miss++;
            steps.push_back(frames);
        }
        displayresults();
    }

    void displayresults()
    {
        for (auto &r : steps)
        {
            for (auto &c : r)
            {
                cout << c << " ";
            }
            cout << "\n";
            sleep(1);
        }

        cout << "Page Hit: " << pg_hit << "\nPage Miss: " << pg_miss << "\n";
        cout << "\nPage hit ratio% : " << (float)(pg_hit * 100) / pages.size() << endl;
        cout << "\nPage miss ratio% : " << (float)(pg_miss * 100) / pages.size() << endl;
    }
};

int main()
{
    cout << "\n-------------------------------Page Replacement Algorithm-----------------------\n";
    cout << "Enter a choice: \n1.FIFO\n2.Optimal Page Replacement\n3.Least Recently Used\n4.Exit\n";
    int ch;
    cin >> ch;
    FIFO f;
    optimal o;
    LRU l;
    switch (ch)
    {
    case 1:
        f.takeinput();
        break;
    case 2:
        o.takeinput();
        break;
    case 3:
        l.takeinput();
        break;
    case 4:
        exit(0);
    default:
        cout << "Please enter a valid option.\n";
        break;
    }
}